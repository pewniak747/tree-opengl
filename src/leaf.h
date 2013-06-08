#ifndef LEAF_H
#define LEAF_H

#include "branch.h"

class Leaf {
  public:
    Leaf(Tree *tree, Branch *branch);
    float length();
    float rootDistance();
    float age();
  private:
    Tree *tree;
    Branch *branch;
    float rootDistanceParameter;
    float created_at;
};

#endif // LEAF_H
