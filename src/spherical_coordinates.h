#ifndef SPHERICAL_COORDINATES_H
#define SPHERICAL_COORDINATES_H

class SphericalCoordinates {
  public:
    SphericalCoordinates(float radialDistance, float polarAngle, float azimuthAngle);
  private:
    float radialDistance;
    float polarAngle;
    float azimuthAngle;
};

#endif // SPHERICAL_COORDINATES_H
