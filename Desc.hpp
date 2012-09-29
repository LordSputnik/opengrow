#ifndef _OG_DESC_HPP_
#define _OG_DESC_HPP_

#include "pch.hpp"

namespace og
{
  struct BranchAttribSet
  {
    float light_attraction; ///< How much the branch should curve towards the light. Value should be between 1 and -1.

    float first_split_distance; ///< The minimum distance to grow before a split can occur.
    float split_frequency; ///< How many times per segment should the branch split.
    float split_angle; ///< The angle to split from the previous branch.

    float segment_length; ///< The length of each segment in the branch.
    float total_length; ///< The total length of the branch.

    uint16_t bud_growth_samples;
  };

  struct BranchDesc
  {
    BranchAttribSet mid;
    BranchAttribSet var;

    BranchDesc(const pugi::xml_node & level);
    BranchDesc(const pugi::xml_node & level, const BranchDesc & defaults);

    private:
      void SetFromNode(const pugi::xml_node & level);
  };

  struct TreeDesc
  {
    // Do this in a similar was to BranchDesc. Then implement tree and branch classes, and bring it up to the old functionality.
    String species;
    uint8_t max_levels;
    std::array<float,2> base_splits;

    std::unique_ptr<BranchDesc> defaults;

    std::vector<std::shared_ptr<BranchDesc> > levels;

    TreeDesc(const pugi::xml_node & tree_root);
  };
}

#endif // _OG_DESC_HPP_

