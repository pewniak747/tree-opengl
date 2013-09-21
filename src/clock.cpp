#include "clock.h"

Clock::Clock() {
  value = 0.0f;
  speed = 0.015f;
}

void Clock::tick() {
  value += speed;
}

void Clock::speedUp(float factor) {
  speed *= factor;
}

void Clock::slowDown(float factor) {
  speed /= factor;
}

float Clock::from(float time) {
  return value - time;
}
