#ifndef SPHERICAL_COORDINATES_H
#define SPHERICAL_COORDINATES_H

#include "glm/glm.hpp"

class SphericalCoordinates {
  public:
    SphericalCoordinates(float radialDistance, float polarAngle, float azimuthAngle);
    SphericalCoordinates(float radialDistance, float polarAngle, float azimuthAngle, float minDistance, float maxDistance, float minPolar, float maxPolar);
    void changeDistance(float delta);
    void changePolar(float delta);
    void changeAzimuth(float delta);
    glm::vec3 toCarthesian();
  private:
    float radialDistance;
    float polarAngle;
    float azimuthAngle;
    float minDistance;
    float maxDistance;
    float minPolar;
    float maxPolar;
};

#endif // SPHERICAL_COORDINATES_H
