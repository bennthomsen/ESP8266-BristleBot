#ifndef Drive_h
#define Drive_h

#include "Arduino.h"
#include "Motor.h"

class Drive
{
  public:
    Drive(int leftpin, int rightpin);
    Motor left;
    Motor right;
    void power(int val);
    void steer(int val);
    void stop();
    void resume();
    void hardLeft();
    void hardRight();

   
  private:
    int _power;
    int _steer;
};

#endif
