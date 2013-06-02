#ifndef TREE_H
#define TREE_H

#include<vector>

class Branch;

class Tree {
  public:
    Tree();
    Branch* getBranch(int x);
    void addBranch();
    int branchCount();
    float clock;
  private:
    std::vector<Branch*> branches;
};

#endif // TREE_H
