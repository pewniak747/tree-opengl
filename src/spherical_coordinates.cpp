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
  this->polarAngle += delta;
  if(this->polarAngle < 0.0f)
    this->polarAngle += 360.0f;
  if(this->polarAngle > 360.0f)
    this->polarAngle -= 360.f;
}

void SphericalCoordinates::changeAzimuth(float delta) {
  this->azimuthAngle += delta;
  if(this->azimuthAngle < 0.0f)
    this->azimuthAngle = 0.0f;
  if(this->azimuthAngle > 90.0f)
    this->azimuthAngle = 90.0f;
}
