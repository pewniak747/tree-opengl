#ifndef LEAF_H
#define LEAF_H

#include "branch.h"

class Leaf {
  public:
    Leaf(Tree *tree, Branch *branch);
    float length();
    float rootDistance();
  private:
    Tree *tree;
    Branch *branch;
    float rootDistanceParameter;
};

#endif // LEAF_H
