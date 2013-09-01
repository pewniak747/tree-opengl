#include "tree.h"
#include "branch.h"

Tree::Tree() {
  clock = new Clock();
}

int Tree::branchCount() {
  return branches.size();
}

Branch* Tree::getBranch(int x) {
  return branches[x];
}

void Tree::grow() {
  float time = this->clock->value;

  if(int(time * 1000) % 200 == 0) {
    this->addBranch();
  }

  if(this->branchCount() > 1 && int(time * 1000) % 10 == 0) {
    this->getBranch(rand() % this->branchCount())->addLeaf();
  }
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
}
