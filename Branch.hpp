#ifndef _OG_BRANCH_HPP_
#define _OG_BRANCH_HPP_

#include "World.hpp"
#include "Desc.hpp"
#include "Cone.hpp"
#include "Sphere.hpp"

namespace og
{
  class Tree;

  /**
   * Represents a single branch of the tree. It maintains a list of the points at the start
   * and end of each segment, and its purpose is mainly to calculate the next segment split
   * point. In the future, it will also hold a series of segment rings, which will be used
   * to render a complete mesh for the tree.
   */
  class Branch
  {
    public:
      /**
      * Constructor. Creates a branch with no parent, at the origin. Used for the first
      * branch in the tree.
      * @param creator The tree that the branch is a part of.
      * @param world The world in which the tree is growing.
      * @param desc The branch description to use to grow the branch.
      */
      Branch(Tree & creator, World & world, BranchDesc & desc);

      /**
      * Constructor. Creates a branch with a specified parent, copying the parent's tip
      * position and orientation.
      * @param creator The tree that the branch is a part of.
      * @param world The world in which the tree is growing.
      * @param desc The branch description to use to grow the branch.
      * @param parent The larger branch that this branch is growing from.
      */
      Branch(Tree & creator, World & world, BranchDesc & desc, const Branch & parent);

      /**
      * Grows a single segment of the branch, taking into account the current state of the
      * world and the growth parameters specified in the branch description.
      */
      void Grow();

      /**
      * Accessor function.
      * @return Whether or not the branch is growing.
      */
      bool growing() const
      {
        return growing_;
      }

      /**
      * Accessor function.
      * @return The current transformation matrix to get from an up vector to the direction and position of the branch tip.
      */
      const cml::matrix33f_c & transform() const
      {
        return transform_;
      }

      /**
      * Accessor function.
      * @return The current position of the tip of the branch.
      */
      const vec3f & bud_pos() const
      {
        return bud_pos_;
      }

      /**
      * Accessor function.
      * @return The level of recursion for this branch. Will be less than the tree's maximum number of levels.
      */
      uint8_t level() const
      {
        return level_;
      }

      void CreateMesh(Ogre::SceneManager* mgr);
      void UpdateMesh(Ogre::SceneManager* mgr);

    private:
      Tree & creator_; ///< The tree that this branch is growing on.
      World & world_; ///< The world in which the branch and rest of the tree are growing.

      BranchDesc & desc_; ///< The branch description to tell the branch how to grow.
      BranchAttribSet attribs_; ///< The specific set of attributes chosen by the branch for the current segment of growth.

      float branch_length_; ///< Current length of the branch. Used for preventing splits until a certain length.
      uint8_t split_type_; ///< A zero indicates this branch doesn't split. 1 indicates it splits an exact number of times per segment. 2 indicates a fractional split.
      uint8_t level_; ///< The level of recursion that this branch is at.

      const Branch* parent_; ///< The parent branch of this branch. Can be nullptr, but only if it was initialized by the tree in that way.

      vec3f bud_pos_; ///< The position of the bud on the branch - the growing tip.
      vec3f bud_dir_; ///< The direction the bud is growing in.
      cml::matrix33f_c transform_; ///< The transformation matrix taking to get to the bud.

      Cone bud_sample_zone_; ///< The area in which to sample for growth points. Updated every Grow().
      Sphere bud_exclusion_zone_; ///< The area to use to tell the world where this branch has been.

      std::vector<vec3f> segments_; ///< The segment positions in the branch. Probably will make a Segment class when we start using rings.

      static uint64_t branch_count_; ///< Static variable used to give each branch a unique identifier string.
      std::string id_string_; ///< The unique identifier string for this branch.

      bool growing_; ///< Whether or not the branch is active/growing.
      bool mesh_created;

      /**
      * Calculates the position for the centre of the next segment start/end, based on the bud sample zone.
      * @return A 3D vector containing the next segment end.
      */
      vec3f GetNextSegmentPos();

      /**
      * Calculates the direction that the bud should grow in, based on the bud sample zone.
      * @return A normalized vector containing the new growth direction.
      */
      vec3f CalculateGrowthVector(const std::vector<vec3i> & in);

      /**
      * Creates one or more new branches at the current bud position.
      * @param num_splits The number of new branches to create. Should be greater than 0.
      */
      void Split(const uint8_t num_splits);

      void SetBranchAttributes();
      void SetSegmentAttributes();
  };

  typedef std::shared_ptr<Branch> BranchPtr;
}

#endif // _OG_BRANCH_HPP_

