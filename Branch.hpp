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
      Branch(Tree & creator, World & world, BranchDesc & desc, Branch* parent);

      void Grow();

      bool growing() const
      {
        return growing_;
      }

      cml::matrix33f_c & transform() const
      {
        return transform_;
      }

    private:
      Tree & creator_;
      World & world_;

      BranchDesc & desc_;
      BranchAttribSet attribs_;

      Branch* parent_;

      bool growing_;

      vec3f bud_pos_;
      vec3f bud_dir_;
      cml::matrix33f_c transform_;

      Cone bud_sample_zone_;
      Sphere bud_exclusion_zone_;

      vec3f GetNextSegmentPos();

      vec3f CalculateGrowthVector(const std::vector<vec3i> & in);

      void SetBranchAttributes();
      void SetSegmentAttributes();
  };

  typedef std::shared_ptr<Branch> BranchPtr;
}

#endif // _OG_BRANCH_HPP_

