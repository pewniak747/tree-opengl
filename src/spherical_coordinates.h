#ifndef SPHERICAL_COORDINATES_H
#define SPHERICAL_COORDINATES_H

#include "glm/glm.hpp"

class SphericalCoordinates {
  public:
    SphericalCoordinates(float radialDistance, float polarAngle, float azimuthAngle);
    void changeDistance(float delta);
    void changePolar(float delta);
    void changeAzimuth(float delta);
    glm::vec3 toCarthesian();
  private:
    float radialDistance;
    float polarAngle;
    float azimuthAngle;
};

#endif // SPHERICAL_COORDINATES_H
