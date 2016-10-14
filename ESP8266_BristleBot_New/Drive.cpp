#include "Drive.h"


Drive::Drive(int leftpin, int rightpin) {
  leftMotor = Motor(leftpin);
  rightMotor = Motor(rightpin);
  _power = 0;
  _steer = 0;
}

void Drive::begin(char* name) {
  _filename = "/" + String(name) + ".json";
  Drive::_loadConfig();
}

void Drive::enable(int value) {
  if(value){
    _enable = true;
    Drive::update();
  }
  else {
    _enable = false;
    Drive::stop();
  } 
}

void Drive::enable() {
  _enable = true;
}

void Drive::disable() {
  _enable = false;
}

void Drive::update() {
  if (_enable) {
  leftMotor.start();
  rightMotor.start();
  }
}

void Drive::setLeftPower(int val) {
  leftMotor.value(val);
  Drive::update();
}

void Drive::setRightPower(int val) {
  rightMotor.value(val);
  Drive::update();
}

void Drive::leftMax(int val) {
  _leftMax = val;
  Drive::_updateConfig();
}

void Drive::rightMax(int val) {
  _rightMax = val;
  Drive::_updateConfig();
}

void Drive::power(int data) {
  _power = data;
  rightMotor.value((_power*_rightMax) >> 8);
  leftMotor.value((_power*_leftMax) >> 8);
  Drive::update();   
}

void Drive::steer(int data) {
  _steer = data;
  if (_steer < 0) {
    leftMotor.value(((256+_steer)*_power*_leftMax) >> 16);  
    rightMotor.value((_power*_rightMax) >> 8);
  }
  else {
    rightMotor.value(((256-_steer)*_power*_rightMax) >> 16);  
    leftMotor.value((_power*_leftMax) >> 8);
  }
  Drive::update(); 
}

void Drive::stop() {
  rightMotor.stop();
  leftMotor.stop(); 
  _endTime = millis()+_timeOuts[0]; 
}

void Drive::forward() {
  Drive::update();
  _endTime = millis()+_timeOuts[1];
}

void Drive::turnLeft() {
  leftMotor.stop();
  rightMotor.fullSpeed(); 
  _endTime = millis()+_timeOuts[2];  
}

void Drive::turnRight() {
  rightMotor.stop();
  leftMotor.fullSpeed(); 
  _endTime = millis()+_timeOuts[3];
}

void Drive::_loadConfig() {
  File configFile = SPIFFS.open(_filename, "r");
  if (configFile) {
    Serial.println("Loading config file: " + _filename);
    size_t size = configFile.size();
    //std::unique_ptr<char[]> buf(new char[size]);
    char data[256];
    //configFile.readBytes(buf.get(), size);
    configFile.readBytes(data,size);
    configFile.close();

    Drive::updateConfig(data);
  } 
  else {
    Serial.println("Creating new config file: " + _filename);
    Drive::_updateConfig();
  }
}

void Drive::_saveConfig(char *data) {
  Serial.print("Writing to file: ");
  Serial.println(_filename);
  File configFile = SPIFFS.open(_filename, "w");
  if (configFile) {
    configFile.print(data);
    configFile.close();
  }
  else Serial.println("Failed to open config file for writing");
}

void Drive::_updateConfig() {
    char data[256];
    Drive::printConfig(data, sizeof(data));
    Drive::_saveConfig(data);
}

void Drive::updateConfig(char *data) {
  Drive::_saveConfig(data);
  StaticJsonBuffer<DRIVE_JSON_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(data);
  if (root.success()) {
    root.printTo(Serial);
    _leftMax = root["leftMax"];
    _rightMax = root["rightMax"];
    int index = 0;
    for (int row=0; row < STATES; row++) {
      for (int col=0; col < EVENTS; col++) {
        index = row*EVENTS+col;
        _transTable[row][col] = root["transitions"][index];
        _actionTable[row][col] = root["actions"][index];
      }
      _timeOuts[row] = root["timeOuts"][row];
    }
  }
  else Serial.println("Failed to parse FSM data");
}

void Drive::printConfig(char* outString, int size) {
  StaticJsonBuffer<DRIVE_JSON_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& transitions = root.createNestedArray("transitions");
  JsonArray& actions = root.createNestedArray("actions");
  JsonArray& timeOuts = root.createNestedArray("timeOuts");

  root["leftMax"] = _leftMax;
  root["rightMax"] = _rightMax;
  
  int index = 0;
  for (int row=0; row < STATES; row++) {
    for (int col=0; col < EVENTS; col++) {
       transitions.add(_transTable[row][col]);
       actions.add(_actionTable[row][col]);
    }
    timeOuts.add(_timeOuts[row]);
  }
  root.printTo(outString,size);      
}

void Drive::setEvent(int event) {
  _event = event;
  _eventServe = true;
}

void Drive::setStart(int start) {
  _endTime = start;
}

bool Drive::fsm_en() {
  return _fsm;
}

void Drive::fsm(int enable) {
  if (enable) {
    Serial.println("Starting Program");
    _fsm = true;
    _event = 0;
    _currentstate = 0;
    _eventServe = true;
    _endTime = MAXTIME;
    _enable = true;
  }
  else {
    _fsm = false;
    Serial.println("Stopping Program");
  }
}

void Drive::loop() {
  if (_fsm){
    if (millis() >= _endTime) Drive::setEvent(TIMEOUT);
      
    if (_eventServe) {
      // Call transition event action
      switch (_actionTable[_currentstate][_event]) {
        case STOP:
          Drive::stop();
          Serial.println("FSM: Stop");
        break;
        case FORW:
          Drive::forward();
          Serial.println("FSM: FORWARD");
        break;
        case TL90:
          Drive::turnLeft();
          Serial.println("FSM: TURNLEFT");
        break;
        case TR90:
          Drive::turnRight();
          Serial.println("FSM: TURNRIGHT");
        break;
      }
  
      _eventServe = false;
      
     // Update state
     _currentstate=_transTable[_currentstate][_event];
    }
  } 
}
  
