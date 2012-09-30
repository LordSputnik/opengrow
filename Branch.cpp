#include "og_pch.hpp"
#include "Branch.hpp"

namespace og
{
  Branch::Branch(Tree & creator, World & world, BranchDesc & desc, Branch* parent) :
    creator_(creator),
    world_(world),
    desc_(desc),
    parent_(parent),
    growing_(true),
    transform_(parent->transform()),
    dir_(transform_vector(parent->transform(), vec3f(0.0f,1.0f,0.0f))),

  {

  }

  void Branch::Grow()
  {
    if ( is_growing_ == false )
      return;

    SetSegmentAttributes();

    segment_.push_back ( bud_->Grow( cur_info_.segment_length ) );

    if( branch_length_ < cur_info_.first_split )
      return;

    if( split_type_ == 1 )
    {
      int splits = static_cast<int> ( cur_info_.split_frequency );
      Split ( splits );
    }
    else if( split_type_ == 2 )
    {
      int splits_i = static_cast<int> ( cur_info_.split_frequency + creator_->split_error(level_) + 0.5f);
      float splits_f = static_cast<float> ( splits_i );

  #ifdef _DEBUG
      assert(splits_i >= 0);
  #endif

      creator_->set_split_error(creator_->split_error(level_) + cur_info_.split_frequency - splits_f, level_);

      Split ( splits_i );
    }
  }

  vec3f Branch::GetNextSegmentPos()
  {
    bud_sample_zone_.Update(pos_, cml::normalize(dir_ - (cml::axis_3D(1) * attribs_.light_attraction)));

    vec3f growth_vector = CalculateGrowthVector(sample_zone_.contained_points());

    cml::matrix33f_c rot = cml::identity_3x3();
    cml::matrix_rotation_vec_to_vec( rot, dir_, growth_vector, true );
    transform_ = rot * transform_;

    dir_ = growth_vector;
    growth_vector *= attribs_.segment_length;
    pos_ = growth_vector += pos_;

//    PointRing ( pos_, transform_, 0.1f - (0.005f * segments++), 8 );

    exclusion_zone_.centre(pos_);

    std::vector<vec3i> & p = exclusion_zone_.contained_points();
    for(size_t i = 0, size = p.size(); i != size; ++i)
    {
       env_->modified_points_[p[i]] = 0.0f;
    }

    return growth_vector;
  }

  vec3f Branch::CalculateGrowthVector ( const std::vector<vec3i> & in )
  {
    vec3f growth_vector(cml::zero_3D());
    std::stack<vec3f> growth_samples;

    getRandomVariation(desc_.mid.bud_growth_samples,desc_.var.bud_growth_samples);

    for(uint16_t i = 0; i != num_growth_samples_; ++i)
    {
      const vec3i & v = in[cml::random_integer(0, in.size() - 1)];

      if(world_->modified_points_.find(v) == world_->modified_points_.end())
      {
        growth_samples.push(intToFloatSpace(v) - position_);
      }
    }

    while( in.empty() == false )
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
      is_growing_ = false;
    }
  }
}
