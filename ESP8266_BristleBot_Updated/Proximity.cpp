#include "Arduino.h"
#include "Proximity.h"

Proximity::Proximity(int emitter_pin, int sensor_pin)
{
  _emitter_pin = emitter_pin;
  _sensor_pin = sensor_pin;
  pinMode(_emitter_pin, OUTPUT);
  pinMode(_sensor_pin, INPUT_PULLUP);
   waitForSensorPtr = &Proximity::waitForSensor;
}

void Proximity::startSensor()
{
  detachInterrupt(digitalPinToInterrupt(_sensor_pin));
  _start = millis();
  attachInterrupt(digitalPinToInterrupt(_sensor_pin), waitForSensorPtr, RISING);
}

void Proximity::waitForSensor()
{
  detachInterrupt(digitalPinToInterrupt(_sensor_pin));
  _value = millis() - _start;
  _limit = (_value > _threshold);
  _done = true;
}
