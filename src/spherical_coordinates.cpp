#include "spherical_coordinates.h"

SphericalCoordinates::SphericalCoordinates(float radialDistance, float polarAngle, float azimuthAngle) {
  this->radialDistance = radialDistance;
  this->polarAngle = polarAngle;
  this->azimuthAngle = azimuthAngle;
}

void SphericalCoordinates::changeDistance(float delta) {
  this->radialDistance += delta;
  if(this->radialDistance < 0)
    this->radialDistance = 0;
}

void SphericalCoordinates::changePolar(float delta) {
  float max = 0.5f * M_PI;
  this->polarAngle += delta;
  if(this->polarAngle < 0.0f)
    this->polarAngle = 0;
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
  return glm::vec3(x, y, z);
}
