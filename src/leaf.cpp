#include "leaf.h"
#include "tree.h"
#include <cmath>

Leaf::Leaf(Tree *t, Branch *b) {
  tree = t;
  branch = b;
  rootDistanceParameter = float(rand() % 100) / 100;
  created_at = tree->clock->value;
}

float Leaf::length() {
  float dynamicLength = log(1+age()/100.0f);
  return std::min(0.25f, dynamicLength);
}

float Leaf::age() {
  return tree->clock->value - created_at;
}

float Leaf::rootDistance() {
  return rootDistanceParameter * branch->length();
}
