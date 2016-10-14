#ifndef Drive_h
#define Drive_h

// Define states
#define STOPPED 0        // Stopped
#define FORWARD 1        // Moving forward
#define TURNL90 2        // Moving forward
#define TURNR90 3        // Moving forward

#define STATES 4

// Define Actions
#define STOP 0        // Stop
#define FORW 1        // Forward
#define TL90 2        // Turn left 90 deg
#define TR90 3        // Turn right 90 deg

// Define events
#define COMMAND 0
#define TIMEOUT 1
#define LEFTSENSORLIMIT 2
#define RIGHTSENSORLIMIT 3
#define SENSORSLIMIT 4
#define CLEAR 5

#define EVENTS 6


#define MAXTIME 0xFFFFFFFF
#define DRIVE_JSON_SIZE (JSON_OBJECT_SIZE(6)+JSON_ARRAY_SIZE(EVENTS*STATES*2 + STATES))

#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>
#include "Motor.h"

class Drive
{
  public:  
    Drive(int leftpin, int rightpin);
    
    void begin(char* name);
    void stop();                // Stops both motors
    void update();              // Sends output power levels to motors when enabled
    void setLeftPower(int val);
    void setRightPower(int val);
    void power(int val);        // Set motor power (Motors respond immediately when enabled)
    void steer(int val);        // Set steering power (Motors respond immediately when enabled)
    void leftMax(int val);      // Set maximum power for the Left Motor
    void rightMax(int val);     // Set maximum power for the Right Motor
    void forward();
    void turnRight();
    void turnLeft();   
    void enable(int value);
    void enable();
    void disable();
    void configJSON(char* outString, int size); 
    void setEvent(int event);
    void setStart(int start);
    bool fsm_en();
    void fsm(int enable);
    void updateConfig(char *data);
    void printConfig(char* outString, int size);
    void loop();

   
  private:
    void _saveConfig(char *data);
    void _loadConfig();
    void _updateConfig();

    Motor leftMotor;
    Motor rightMotor;
    int _power;
    int _steer;
    volatile unsigned long _endTime;
    bool _now = false;
    volatile int _action = 0;
    int _leftMax = 255;
    int _rightMax = 255;
    bool _auto = false;
    String _filename = "";
    volatile bool _enable = false;

    volatile bool _eventServe = false;
    volatile int _event = 0;
    bool _fsm = false;
    int _currentstate = 0;
    int _actionTable [STATES][EVENTS];                         
    int _transTable [STATES][EVENTS];
    int _timeOuts [STATES];

};  //endof Drive class
#endif
