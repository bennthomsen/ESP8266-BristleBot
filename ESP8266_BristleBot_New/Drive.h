#ifndef Drive_h
#define Drive_h

#define STOP 0
#define FORWARD 1
#define TURNLEFT 2
#define TURNRIGHT 3
#define TURN180 4

#define MAXTIME 0xFFFFFFFF
#define SENSORDATA_JSON_SIZE (JSON_OBJECT_SIZE(5))

#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>
#include "Motor.h"

class Drive
{
  public:
    Motor left;
    Motor right;
    
    Drive(int leftpin, int rightpin);
    
    void begin(char* name);
    void power(int val);
    void steer(int val);
    void leftMax(int val);
    void rightMax(int val);
    void forward();
    void turnRight();
    void turnLeft();
    void turn180();
    int delayLeft();
    int delayRight();
    int delay180();
    void delayLeft(int value);
    void delayRight(int value);
    void delay180(int value);
    void stop();
    void hardLeft();
    void hardRight();
    void autonomousEnable(int enable);
    void stopAutonomous();
    void autonomous();
    bool auto_en();
    void set();                // sets motor drive output signals
    void enable();
    void enable(int value);
    void disable();
    void configJSON(char* outString, int size); 

   
  private:
    int _power;
    int _steer;
    unsigned long _start;
    bool _now = false;
    volatile int _action = 0;
    int _leftMax = 255;
    int _rightMax = 255;
    int _delayLeft = 500;
    int _delayRight = 500;
    int _delay180 =1000;
    bool _auto = false;
    String _filename = "";
    volatile bool _enable = false;

    void _saveConfig();
    void _loadConfig();
};

#endif
