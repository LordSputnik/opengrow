#include "og_pch.hpp"
#include "Tree.hpp"

namespace og
{
  Tree::Tree(const TreeDesc & desc, World & world) :
    desc_(desc),
    world_(world),
    mgr_(nullptr)
  {
    split_error.resize(desc_.max_levels,0.0f);

    BranchPtr ptr(new Branch(*this, world_, *desc_.levels[0]));

    branches_.push_back(ptr);
    active_branches_.push_back(ptr);

    next_to_grow = active_branches_.begin();
  }

  void Tree::Grow()
  {
    if(next_to_grow == active_branches_.end())
    {
      while(branch_add_queue_.empty() == false)
      {
        active_branches_.push_back(branch_add_queue_.front());
        branch_add_queue_.pop();
      }

      next_to_grow = active_branches_.begin();
    }

    (*next_to_grow)->Grow();
    (*next_to_grow)->UpdateMesh(mgr_);
    ++next_to_grow;
  }

  Branch* Tree::CreateBranch(const Branch & parent)
  {
    BranchPtr ptr(new Branch(*this, world_, (*desc_.levels[parent.level()+1]), parent));
    ptr->CreateMesh(mgr_);

    branches_.push_back(ptr);
    branch_add_queue_.push(ptr);

    return ptr.get();
  }
}
