#ifndef BRANCH_H
#define BRANCH_H

class Tree;

class Branch {
  public:
    Branch(Tree *t, float x,  int y);
    float length();
    float radius();
    float rootDistance();
    float age();
    std::vector<int> parents();
    float created_at;
    float angle;
    float rootDistanceParameter;
    float direction;
    int parent;
    int level;
  private:
    Tree *tree;
    float lengthAt(float time);
    float radiusAt(float time);
};

#endif // BRANCH_H
