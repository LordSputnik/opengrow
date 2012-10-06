#ifndef _OG_BRANCH_HPP_
#define _OG_BRANCH_HPP_

#include "World.hpp"
#include "Desc.hpp"
#include "Cone.hpp"
#include "Sphere.hpp"

namespace og
{
  class Tree;

  class Branch
  {
    public:
      Branch(Tree & creator, World & world, BranchDesc & desc);
      Branch(Tree & creator, World & world, BranchDesc & desc, const Branch & parent);

      void Grow();

      bool growing() const
      {
        return growing_;
      }

      const cml::matrix33f_c & transform() const
      {
        return transform_;
      }

      const vec3f & bud_pos() const
      {
        return bud_pos_;
      }

      uint8_t level() const
      {
        return level_;
      }

      void CreateMesh(Ogre::SceneManager* mgr);
      void UpdateMesh(Ogre::SceneManager* mgr);

    private:
      Tree & creator_;
      World & world_;

      BranchDesc & desc_;
      BranchAttribSet attribs_;

      const Branch* parent_;

      bool growing_;

      vec3f bud_pos_;
      vec3f bud_dir_;
      cml::matrix33f_c transform_;

      Cone bud_sample_zone_;
      Sphere bud_exclusion_zone_;

      std::vector<vec3f> segments_;

      float branch_length_;

      uint8_t split_type_;

      uint8_t level_;

      static uint32_t branch_count_;

      bool mesh_created;

      std::string id_string_;

      vec3f GetNextSegmentPos();

      vec3f CalculateGrowthVector(const std::vector<vec3i> & in);
      void Split(const uint8_t num_splits);

      void SetBranchAttributes();
      void SetSegmentAttributes();
  };

  typedef std::shared_ptr<Branch> BranchPtr;
}

#endif // _OG_BRANCH_HPP_

