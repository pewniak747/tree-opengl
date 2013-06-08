#ifndef BRANCH_H
#define BRANCH_H

#include<vector>

class Tree;

class Leaf;

class Branch {
  public:
    Branch(Tree *t, float x,  int y);
    float length();
    float radius();
    float rootDistance();
    float age();
    bool isTrunk();
    std::vector<int> parents();
    std::vector<Leaf*> leaves;
    float created_at;
    float angle;
    float rootDistanceParameter;
    float direction;
    int parent;
    int level;
    void addLeaf();
  private:
    Tree *tree;
    float lengthAt(float time);
    float radiusAt(float time);
    Branch *parentBranch();
};

#endif // BRANCH_H
