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
  float max = this->maxDistance;
  float min = this->minDistance;
  this->radialDistance += delta;
  if(this->radialDistance < min)
    this->radialDistance = min;
  if(this->radialDistance > max)
    this->radialDistance = max;
}

void SphericalCoordinates::changePolar(float delta) {
  float max = this->maxPolar;
  float min = this->minPolar;
  this->polarAngle += delta;
  if(this->polarAngle < min)
    this->polarAngle = min;
  if(this->polarAngle > max)
    this->polarAngle = max;
}

void SphericalCoordinates::changeAzimuth(float delta) {
  float max = 2 * M_PI;
  this->azimuthAngle += delta;
  if(this->azimuthAngle < 0.0f)
    this->azimuthAngle += max;
  if(this->azimuthAngle > max)
    this->azimuthAngle -= max;
}

glm::vec3 SphericalCoordinates::toCarthesian() {
  float x = this->radialDistance * sin(this->polarAngle) * cos(this->azimuthAngle);
  float y = this->radialDistance * sin(this->polarAngle) * sin(this->azimuthAngle);
  float z = this->radialDistance * cos(this->polarAngle);
  return glm::vec3(x, z, y);
}
