#ifndef BRANCH_H
#define BRANCH_H

class Tree;

class Branch {
  public:
    Branch(Tree *t, float x,  int y);
    float length();
    float radius();
    std::vector<int> parents();
    float created_at;
    float angle;
    float direction;
    int parent;
    int level;
  private:
    Tree *tree;
};

#endif // BRANCH_H
