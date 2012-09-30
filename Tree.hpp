#ifndef _OG_TREE_HPP_
#define _OG_TREE_HPP_

#include "Desc.hpp"
#include "Branch.hpp"

namespace og
{
  class World;

  class Tree
  {
    friend class Branch;

    public:
      /** Constructs a tree based on the provided position, to be grown in the given world.
       * @param desc The tree description to use to grow the tree.
       * @param world The world in which to grow.
       */
      Tree(const TreeDesc & desc, World* const world);

      /**
       * Iterates through all the active branches and tells them to grow.
       */
      void Grow();

    protected:
      /**
       * Creates a branch as a child of the provided parent branch, and then adds it to the
       * list of active branches.
       * @param parent The branch off of which the new branch will grow.
       */
      Branch* CreateBranch(Branch* const parent);

    private:
      const TreeDesc & desc_;

      std::vector<BranchPtr> branches_;
      std::list<BranchPtr> active_branches_;

      World* const world_; /// A constant pointer to the world to grow in.
  };
}

#endif // _OG_TREE_HPP_

