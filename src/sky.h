#ifndef SKY_H
#define SKY_H

#include <vector>

#include "clock.h"
#include "glm/glm.hpp"

class Sky {
  public:
    Sky(Clock *clock);
    glm::vec3 color();
  private:
    Clock *clock;
    std::vector<glm::vec3> colors;
    std::vector<float> durations;
};

#endif // SKY_H
