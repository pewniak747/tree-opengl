#include "spherical_coordinates.h"

SphericalCoordinates::SphericalCoordinates(float radialDistance, float azimuthAngle, float polarAngle) {
  this->radialDistance = radialDistance;
  this->polarAngle = polarAngle;
  this->azimuthAngle = azimuthAngle;
  this->minDistance = 0.0f;
  this->maxDistance = FLT_MAX;
  this->minPolar = 0.0f;
  this->maxPolar = 0.5f * M_PI;
}

SphericalCoordinates::SphericalCoordinates(float radialDistance, float azimuthAngle, float polarAngle, float minDistance, float maxDistance, float minPolar, float maxPolar) {
  this->radialDistance = radialDistance;
  this->polarAngle = polarAngle;
  this->azimuthAngle = azimuthAngle;
  this->minDistance = minDistance;
  this->maxDistance = maxDistance;
  this->minPolar = minPolar;
  this->maxPolar = maxPolar;
}

void SphericalCoordinates::changeDistance(float delta) {
  float max = maxDistance;
  float min = minDistance;
  radialDistance += delta;
  if(radialDistance < min)
    radialDistance = min;
  if(radialDistance > max)
    radialDistance = max;
}

void SphericalCoordinates::changePolar(float delta) {
  float max = maxPolar;
  float min = minPolar;
  polarAngle += delta;
  if(polarAngle < min)
    polarAngle = min;
  if(polarAngle > max)
    polarAngle = max;
}

void SphericalCoordinates::changeAzimuth(float delta) {
  float max = 2 * M_PI;
  azimuthAngle += delta;
  if(azimuthAngle < 0.0f)
    azimuthAngle += max;
  if(azimuthAngle > max)
    azimuthAngle -= max;
}

glm::vec3 SphericalCoordinates::toCarthesian() {
  float x = radialDistance * sin(polarAngle) * cos(azimuthAngle);
  float y = radialDistance * sin(polarAngle) * sin(azimuthAngle);
  float z = radialDistance * cos(polarAngle);
  return glm::vec3(x, z, y);
}
