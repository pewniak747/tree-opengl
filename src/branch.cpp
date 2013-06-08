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
  
  //angle = 45;
  //direction = 0;
}

float Branch::length() {
  return log(1+(tree->clock - created_at)/(10*(1+level)));
  //return 1.0f;
}

float Branch::radius() {
  return log(1+(tree->clock - created_at)/(100*(level+1)));
}

float Branch::rootDistance() {
  float parentLength = tree->getBranch(parent)->length();
  return rootDistanceParameter * parentLength;
  //float dynamicLength = log(1+(tree->clock - tree->getBranch(parent)->created_at)/10);
  //return std::min(parentLength, dynamicLength);
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

