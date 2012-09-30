#include "og_pch.hpp"
#include "Tree.hpp"

Tree(const TreeDesc & desc, World* const world) :
  desc_(desc),
  world_(world)
{

}

void Grow()
{
  for(BranchPtr & ptr : active_branches_)
  {
    ptr->Grow();
  }
}

Branch* CreateBranch(Branch* const parent)
{
  BranchPtr ptr(new Branch());

  branches_.push_back(ptr);
  active_branches_.push_back(ptr);
}
