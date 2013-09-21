#include "sky.h"

#define ONE_DAY 180.0f

Sky::Sky(Clock *clock) {
  this->clock = clock;
  this->colors.push_back(glm::vec3(0.53f, 0.8f, 1.0f));
  this->colors.push_back(glm::vec3(0.53f, 0.8f, 1.0f));
  this->colors.push_back(glm::vec3(0.98f, 0.80f, 0.01f));
  this->colors.push_back(glm::vec3(0.06f, 0.01f, 0.12f));
  this->colors.push_back(glm::vec3(0.06f, 0.01f, 0.12f));
  this->durations.push_back(ONE_DAY / 2);
  this->durations.push_back(ONE_DAY / 2 + 10.0f);
  this->durations.push_back(ONE_DAY / 2 + 20.0f);
  this->durations.push_back(ONE_DAY - 10.0f);
  this->durations.push_back(ONE_DAY);
}

glm::vec3 Sky::color() {
  float time = fmod(clock->value, ONE_DAY);
  int colorIndex;
  for(colorIndex = 0; durations[colorIndex] < time; colorIndex++);
  int nextColorIndex = (colorIndex + 1) % colors.size();
  int previousColorIndex = (colorIndex + colors.size() - 1) % colors.size();
  glm::vec3 currentColor = colors[colorIndex];
  glm::vec3 nextColor = colors[nextColorIndex];
  float diffTime = time - fmod(durations[previousColorIndex], ONE_DAY);
  float maxDiffTime = (durations[colorIndex] - fmod(durations[previousColorIndex], ONE_DAY));
  float diffFactor = diffTime / maxDiffTime;
  float diffRed = (nextColor.r - currentColor.r) * diffFactor;
  float diffGreen = (nextColor.g - currentColor.g) * diffFactor;
  float diffBlue = (nextColor.b - currentColor.b) * diffFactor;
  return glm::vec3(currentColor.r + diffRed, currentColor.g + diffGreen, currentColor.b + diffBlue);
}
