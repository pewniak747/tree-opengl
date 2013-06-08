#ifndef TREE_H
#define TREE_H

#include<vector>

#include "clock.h"

class Branch;

class Tree {
  public:
    Tree();
    Branch* getBranch(int x);
    void addBranch();
    int branchCount();
    Clock *clock;
  private:
    std::vector<Branch*> branches;
};

#endif // TREE_H
