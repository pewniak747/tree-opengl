#ifndef TREE_H
#define TREE_H

#include<vector>

#include "clock.h"

class Branch;

class Tree {
  public:
    Tree(Clock *clock);
    Branch* getBranch(int x);
    Branch* getTrunk();
    void grow();
    int branchCount();
    Clock *clock;
  private:
    std::vector<Branch*> branches;
    void addBranch();
    float lastGrowTime;
    float lastLeafGrowTime;
};

#endif // TREE_H
