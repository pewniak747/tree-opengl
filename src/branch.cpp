#include<vector>
#include<math.h>
#include "branch.h"
#include "tree.h"
#include "leaf.h"

Branch::Branch(Tree *t, float x, int y) {
  created_at = x;
  parent = y;
  tree = t;
  angle = (rand() % 180 - 90);
  rootDistanceParameter = (rand() % 100 + 50.0f) / 200.0f;
  direction = rand() % 360;
  if(parent >= 0) {
    level = parentBranch()->level + 1;
  }
  else level = 0;
}

float Branch::length() {
  return lengthAt(age());
}

bool Branch::isTrunk() {
  return parent == -1;
}

float Branch::radius() {
  float baseRadius = radiusAt(age());
  if(isTrunk()) {
    return baseRadius;
  }
  else {
    float parentRadius = parentBranch()->radius();
    float parentLength = parentBranch()->length();
    float distance = rootDistance();
    float allowedRadius = ((parentLength-distance)*parentRadius)/sqrt(pow(parentRadius, 2)+pow(parentLength, 2));
    return std::min(baseRadius, allowedRadius);
  }
}

Branch* Branch::parentBranch() {
  return tree->getBranch(parent);
}

float Branch::age() {
  return tree->clock->from(created_at);
}

float Branch::rootDistance() {
  float parentLength = tree->getBranch(parent)->length();
  return rootDistanceParameter * parentLength;
}

float Branch::lengthAt(float time) {
  return log(1+time/(10*(level+1)));
}

float Branch::radiusAt(float time) {
  return log(1+time/(200*(level+1)));
}

std::vector<int> Branch::parents() {
  std::vector<int> result;
  int par = parent;
  while(par >= 0) {
    result.push_back(par);
    par = tree->getBranch(par)->parent;
  }
  return result;
}

void Branch::addLeaf() {
  if(level > 2 && leaves.size() < 5) {
    Leaf *newLeaf = new Leaf(tree, this);
    leaves.push_back(newLeaf);
    printf("Created leaf: id = %d, branch = %d\n", leaves.size(), 0);
  }
}
