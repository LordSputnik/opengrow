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
      Tree(const TreeDesc & desc, World & world);

      /**
       * Iterates through all the active branches and tells them to grow.
       */
      void Grow();

      std::string species()
      {
        return desc_.species;
      }

      uint16_t branches()
      {
        return branches_.size();
      }

      void SetOgreSceneManager(Ogre::SceneManager* mgr)
      {
        mgr_ = mgr;
      }

    protected:
      /**
       * Creates a branch as a child of the provided parent branch, and then adds it to the
       * list of active branches.
       * @param parent The branch off of which the new branch will grow.
       */
      Branch* CreateBranch(const Branch & parent);

      std::vector<float> split_error;

    private:
      const TreeDesc & desc_;

      std::vector<BranchPtr> branches_;
      std::list<BranchPtr> active_branches_;
      std::list<BranchPtr>::iterator next_to_grow;
      std::queue<BranchPtr> branch_add_queue_;

      World & world_; /// A constant pointer to the world to grow in.

      Ogre::SceneManager* mgr_;
  };
}

#endif // _OG_TREE_HPP_

