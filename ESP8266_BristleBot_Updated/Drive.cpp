#include "Arduino.h"
#include "Motor.h"
#include "Drive.h"


Drive::Drive(int leftpin, int rightpin)
{
  left = Motor(leftpin);
  right = Motor(rightpin);
  _power = 0;
  _steer = 0;
}


void Drive::power(int data)
{
  _power = data;
  right.value((_power*right.maximum()) >> 8);
  left.value((_power*left.maximum()) >> 8);   
}

void Drive::steer(int data)
{
  _steer = data;
  if (_steer < 0) {
    left.value(((256+_steer)*_power*left.maximum()) >> 16);  
    right.value((_power*right.maximum()) >> 8);
  }
  else {
    right.value(((256-_steer)*_power*right.maximum()) >> 16);  
    left.value((_power*left.maximum()) >> 8);
  }
}

void Drive::stop()
{
  right.stop();
  left.stop();  
}

void Drive::resume()
{
  right.resume();
  left.resume();  
}

void Drive::hardRight()
{
  right.stop();
  left.fullSpeed();  
}

void Drive::hardLeft()
{
  left.stop();
  right.fullSpeed();  
}
