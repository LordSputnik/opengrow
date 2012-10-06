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
  }

  void Tree::Grow()
  {
    for(BranchPtr & ptr : active_branches_)
    {
      ptr->Grow();
      ptr->UpdateMesh(mgr_);
    }
  }

  Branch* Tree::CreateBranch(const Branch & parent)
  {
    BranchPtr ptr(new Branch(*this, world_, (*desc_.levels[parent.level()+1]), parent));
    ptr->CreateMesh(mgr_);

    branches_.push_back(ptr);
    active_branches_.push_back(ptr);
  }
}
