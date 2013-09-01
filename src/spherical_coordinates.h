#ifndef SPHERICAL_COORDINATES_H
#define SPHERICAL_COORDINATES_H

class SphericalCoordinates {
  public:
    SphericalCoordinates(float radialDistance, float polarAngle, float azimuthAngle);
    void changeDistance(float delta);
    void changePolar(float delta);
    void changeAzimuth(float delta);
  private:
    float radialDistance;
    float polarAngle;
    float azimuthAngle;
};

#endif // SPHERICAL_COORDINATES_H
