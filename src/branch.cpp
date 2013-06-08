#include<vector>
#include<math.h>
#include "branch.h"
#include "tree.h"

Branch::Branch(Tree *t, float x, int y) {
  created_at = x;
  parent = y;
  tree = t;
  angle = (rand() % 180 - 90);
  rootDistanceParameter = (rand() % 100 + 50.0f) / 200.0f;
  direction = rand() % 360;
  if(parent >= 0) {
    level = tree->getBranch(parent)->level + 1;
  }
  else level = 0;
}

float Branch::length() {
  return log(1+age()/(10*(1+level)));
}

float Branch::radius() {
  return log(1+age()/(100*(level+1)));
}

float Branch::age() {
  return tree->clock->from(created_at);
}

float Branch::rootDistance() {
  float parentLength = tree->getBranch(parent)->length();
  return rootDistanceParameter * parentLength;
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
