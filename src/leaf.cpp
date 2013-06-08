#include "leaf.h"

Leaf::Leaf(Tree *t, Branch *b) {
  tree = t;
  branch = b;
  rootDistanceParameter = float(rand() % 100) / 100;
}

float Leaf::length() {
  return 0.1f;
}

float Leaf::rootDistance() {
  return rootDistanceParameter * branch->length();
}
