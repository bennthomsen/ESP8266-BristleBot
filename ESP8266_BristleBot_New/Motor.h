#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor
{
  public:
    Motor();
    Motor(int pin);
    void value(int val);
    int value();
    void maximum(int val);
    int maximum();
    void fullSpeed();
    void stop();
    void start();
   
  private:
    int _pin;
    int _value;
    int _maximum;
};

#endif
