#ifndef CLOCK_H
#define CLOCK_H

class Clock {
  public:
    Clock();
    void tick();
    float from(float time);
    float value;
  private:
    float speed;
};

#endif // CLOCK_H
