#ifndef Proximity_h
#define Proximity_h

#include "Arduino.h"

class Proximity
{
  public:
    Proximity(int emitter_pin, int sensor_pin);
    void startSensor();
    void waitForSensor();
    static void startEmitter();
   
  private:
    int _emitter_pin;
    int _sensor_pin;
    long _start;
    int _value;
    int _threshold;
    bool _limit;
    bool _done;
    void (*waitForSensorPtr) (void);
};

#endif
