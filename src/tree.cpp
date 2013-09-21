#include "tree.h"
#include "branch.h"

Tree::Tree(Clock *clock) {
  this->clock = clock;
  this->lastGrowTime = clock->value;
  this->lastLeafGrowTime = clock->value;
}

int Tree::branchCount() {
  return branches.size();
}

Branch* Tree::getBranch(int x) {
  return branches[x];
}

void Tree::grow() {
  float time = clock->value;
  float timeDelta = time - lastGrowTime;
  float leafTimeDelta = time - lastLeafGrowTime;
  float branchFrequency = 1.0f;
  float leafFrequency = 5.0f;

  int branchesToAdd = int(timeDelta * branchFrequency);
  if(branchCount() == 0) branchesToAdd = 1;
  int leavesToAdd = int(leafTimeDelta * leafFrequency);

  for(int i = 0; i < branchesToAdd; i++) {
    addBranch();
  }

  if(branchCount() > 1) {
    for(int i = 0; i < leavesToAdd; i++) {
      getBranch(rand() % branchCount())->addLeaf();
      this->lastLeafGrowTime = time;
    }
  }
}

Branch *Tree::getTrunk() {
  if(branchCount() > 1)
    return getBranch(0);
  else
    return new Branch(this, clock->value, -1);
}

void Tree::addBranch() {
  Branch *newBranch;
  if(branches.size() == 0) {
    newBranch = new Branch(this, clock->value, -1);
    newBranch->direction = 0;
    newBranch->angle = 0;
  }
  else {
    int maxLevel = 0;
    for(int i = 0; i < branches.size(); i++)
      if(branches[i]->level > maxLevel) maxLevel = branches[i]->level;
    if(maxLevel > 3) maxLevel = 3;
    int randLevel = rand() % (maxLevel+1);
    std::vector<int> candidateBranches;
    for(int i = 0; i < branches.size(); i++)
      if(branches[i]->level == randLevel) candidateBranches.push_back(i);
    int choosenBranch = candidateBranches[rand() % candidateBranches.size()];
    newBranch = new Branch(this, clock->value, choosenBranch);
  }
  printf("Created branch: id = %d, parent = %d, level = %d.\n", branchCount(), newBranch->parent, newBranch->level);
  branches.push_back(newBranch);
  this->lastGrowTime = clock->value;
}
