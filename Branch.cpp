#include "og_pch.hpp"
#include "Branch.hpp"
#include "Tree.hpp"

namespace og
{
  uint32_t Branch::branch_count_ = 0;

  Branch::Branch(Tree & creator, World & world, BranchDesc & desc) :
    creator_(creator),
    world_(world),
    desc_(desc),
    parent_(nullptr),
    growing_(true),
    transform_(cml::identity_3x3()),
    bud_pos_(cml::zero_3D()),
    bud_dir_(cml::y_axis_3D()),
    bud_sample_zone_(cml::zero_3D(), cml::y_axis_3D(), 0.3f, 45.0f, 0.1f),
    bud_exclusion_zone_(cml::zero_3D(), 0.1f),
    level_(0),
    mesh_created(false)
  {
    SetBranchAttributes();
    ++branch_count_;

    std::stringstream os;
    os << branch_count_;
    id_string_ = os.str();

    segments_.push_back(bud_pos_);
  }

  Branch::Branch(Tree & creator, World & world, BranchDesc & desc, const Branch & parent) :
    creator_(creator),
    world_(world),
    desc_(desc),
    parent_(&parent),
    growing_(true),
    transform_(parent.transform()),
    bud_pos_(parent.bud_pos()),
    bud_dir_(transform_vector(parent.transform(),cml::y_axis_3D())),
    bud_sample_zone_(bud_pos_, bud_dir_, 0.3f, 45.0f, 0.1f),
    bud_exclusion_zone_(bud_pos_, 0.1f),
    level_(parent.level()+1),
    mesh_created(false)
  {
    SetBranchAttributes();
    ++branch_count_;

    std::stringstream os;
    os << branch_count_;
    id_string_ = os.str();

    segments_.push_back(bud_pos_);
  }

  void Branch::CreateMesh(Ogre::SceneManager* mgr)
  {
    Ogre::ManualObject* mo = mgr->createManualObject ("TreeSkeleton"+id_string_);
    mo->estimateVertexCount(100);
    mo->setDynamic(true);
    mo->begin("", Ogre::RenderOperation::OT_LINE_STRIP);

    mo->position( Ogre::Vector3::ZERO );

    mo->end();

    mgr->getSceneNode("Model")->createChildSceneNode()->attachObject(mo);

    mesh_created = true;
  }

  void Branch::UpdateMesh(Ogre::SceneManager* mgr)
  {
    if(mesh_created == false)
      CreateMesh(mgr);

    Ogre::ManualObject* mo = mgr->getManualObject("TreeSkeleton"+id_string_);

    mo->beginUpdate(0);

    for(const vec3f & v : segments_)
    {
      mo->position(v[0],v[1],v[2]);
    }

    mo->end();
  }

  void Branch::Split(const uint8_t num_splits)
  {
    if ( level_ >= creator_.desc_.max_levels - 1 )
      return;

    attribs_.split_angle = getRandomVariation ( desc_.mid.split_angle, desc_.var.split_angle );
    attribs_.split_angle = std::min ( attribs_.split_angle, 80.0f );

    float radial_angle = getRandomVariation( 180.0f, 180.0f );
    float increment_angle = 360.0f / num_splits;

    for(uint8_t i = 0; i != num_splits; ++i)
    {
      cml::matrix33f_c transform ( transform_ );
      cml::matrix_rotate_about_local_y ( transform, cml::rad ( radial_angle ) );
      cml::matrix_rotate_about_local_x ( transform, cml::rad ( attribs_.split_angle ) );

      vec3f dir ( transform_vector ( transform, cml::y_axis_3D() ) );
      creator_.CreateBranch(*this);
      radial_angle += getRandomVariation ( increment_angle, increment_angle * 0.5f );
    }
  }

  void Branch::Grow()
  {
    if ( growing_ == false )
      return;

    SetSegmentAttributes();

    segments_.push_back ( GetNextSegmentPos() );

    if( branch_length_ < attribs_.first_split_distance )
      return;

    if( split_type_ == 1 )
    {
      int splits = static_cast<int> ( attribs_.split_frequency );
      Split ( splits );
    }
    else if( split_type_ == 2 )
    {
      int splits_i = static_cast<int> ( attribs_.split_frequency + creator_.split_error[level_] + 0.5f);
      float splits_f = static_cast<float> ( splits_i );

  #ifdef _DEBUG
      assert(splits_i >= 0);
  #endif

      creator_.split_error[level_] += attribs_.split_frequency - splits_f;

      Split ( splits_i );
    }

  }

  vec3f Branch::GetNextSegmentPos()
  {
    bud_sample_zone_.Update(bud_pos_, cml::normalize(bud_dir_ - (cml::axis_3D(1) * attribs_.light_attraction)));

    vec3f growth_vector = CalculateGrowthVector(bud_sample_zone_.contained_points());

    cml::matrix33f_c rot = cml::identity_3x3();
    cml::matrix_rotation_vec_to_vec( rot, bud_dir_, growth_vector, true );
    transform_ = rot * transform_;

    bud_dir_ = growth_vector;
    growth_vector *= attribs_.segment_length;
    bud_pos_ = growth_vector += bud_pos_;

//    PointRing ( pos_, transform_, 0.1f - (0.005f * segments++), 8 );

    bud_exclusion_zone_.centre(bud_pos_);

    std::vector<vec3i> & p = bud_exclusion_zone_.contained_points();
    for(size_t i = 0, size = p.size(); i != size; ++i)
    {
       world_.modified_points_[p[i]] = 0.0f;
    }

    return growth_vector;
  }

  vec3f Branch::CalculateGrowthVector ( const std::vector<vec3i> & in )
  {
    vec3f growth_vector(cml::zero_3D());
    std::stack<vec3f> growth_samples;

    attribs_.bud_growth_samples = getRandomVariation(desc_.mid.bud_growth_samples,desc_.var.bud_growth_samples);

    for(uint16_t i = 0; i != attribs_.bud_growth_samples; ++i)
    {
      const vec3i & v = in[cml::random_integer(0, in.size() - 1)];

      if(world_.modified_points_.find(v) == world_.modified_points_.end())
      {
        growth_samples.push(intToFloatSpace(v) - bud_pos_);
      }
    }

    while( growth_samples.empty() == false )
    {
      growth_vector += growth_samples.top();
      growth_samples.pop();
    }

    if ( growth_vector.length_squared() != 0.0f )
      growth_vector.normalize();

    return growth_vector;
  }

  void Branch::SetBranchAttributes()
  {
    attribs_.split_frequency = getRandomVariation(desc_.mid.split_frequency, desc_.var.split_frequency);

    if(fabs(attribs_.split_frequency) < 0.01f)
    {
      split_type_ = 0;
    }
    else if(fabs(floor(attribs_.split_frequency) - attribs_.split_frequency) < 0.01f)
    {
      split_type_ = 1;
    }
    else
    {
      split_type_ = 2;
    }

    attribs_.total_length = getRandomVariation(desc_.mid.total_length, desc_.var.total_length);

    attribs_.first_split_distance = getRandomVariation(desc_.mid.first_split_distance, desc_.var.first_split_distance);
    attribs_.first_split_distance *= attribs_.total_length; //Relative to total length - multiple to get the actual distance.

    attribs_.bud_growth_samples = getRandomVariation(desc_.mid.bud_growth_samples, desc_.var.bud_growth_samples);

    attribs_.light_attraction = getRandomVariation(desc_.mid.light_attraction, desc_.var.light_attraction);
  }

  void Branch::SetSegmentAttributes()
  {
    attribs_.segment_length = std::min(attribs_.total_length, getRandomVariation(desc_.mid.segment_length, desc_.var.segment_length));
    attribs_.total_length -= attribs_.segment_length;
    branch_length_ += attribs_.segment_length;

    if(fabs(attribs_.total_length) < 0.001f)
    {
      growing_ = false;
    }
  }
}
