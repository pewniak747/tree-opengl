#include "tree.h"
#include "branch.h"

Tree::Tree(Clock *_clock) {
  clock = _clock;
  lastGrowTime = clock->value;
  lastLeafGrowTime = clock->value;
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
  int maxBranches = 150;

  int branchesToAdd = int(timeDelta * branchFrequency);
  if(branchCount() == 0) branchesToAdd = 1;
  if(branchCount() > maxBranches) branchesToAdd = 0;
  int leavesToAdd = int(leafTimeDelta * leafFrequency);

  for(int i = 0; i < branchesToAdd; i++) {
    addBranch();
  }

  if(branchCount() > 1) {
    for(int i = 0; i < leavesToAdd; i++) {
      getBranch(rand() % branchCount())->addLeaf();
      lastLeafGrowTime = time;
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
    int branchesPerLevel[] = { 0, 0, 0, 0, 0, 0 };
    for(int i = 0; i < branches.size(); i++) {
      branchesPerLevel[branches[i]->level]++;
    }
    int chosenLevel;
    for(chosenLevel = 0; branchesPerLevel[chosenLevel] >= 1<<(2*chosenLevel); chosenLevel++);
    chosenLevel--;
    std::vector<int> candidateBranches;
    for(int i = 0; i < branches.size(); i++)
      if(branches[i]->level == chosenLevel) candidateBranches.push_back(i);
    int chosenParent = candidateBranches[rand() % candidateBranches.size()];
    newBranch = new Branch(this, clock->value, chosenParent);
  }
  printf("Created branch: id = %d, parent = %d, level = %d.\n", branchCount(), newBranch->parent, newBranch->level);
  branches.push_back(newBranch);
  lastGrowTime = clock->value;
}
