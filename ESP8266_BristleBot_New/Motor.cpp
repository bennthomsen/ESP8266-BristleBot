#include "Arduino.h"
#include "Motor.h"

Motor::Motor()
{
  _maximum = 255;
}

Motor::Motor(int pin)
{
  pinMode(pin, OUTPUT);
  analogWriteFreq(400);             // Set frequency for PWM
  analogWrite(_pin,0);
  _pin = pin;
}

void Motor::value(int data)
{
  _value = data;
}

int Motor::value()
{
  return _value;
}

void Motor::maximum(int data)
{
  _maximum = data;
}

int Motor::maximum()
{
  return _maximum;
}

void Motor::stop()
{
  analogWrite(_pin,0);
}

void Motor::start()
{
  analogWrite(_pin,_value);
}

void Motor::fullSpeed()
{
  analogWrite(_pin,_maximum);
}

