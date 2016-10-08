#include "Drive.h"


Drive::Drive(int leftpin, int rightpin) {
  left = Motor(leftpin);
  right = Motor(rightpin);
  _power = 0;
  _steer = 0;
}

void Drive::begin(char* name) {
  _filename = "/" + String(name) + ".json";
  Drive::_loadConfig();
}

void Drive::enable() {
  _enable = true;
  //Serial.println("_enable: true");
}

void Drive::disable() {
  _enable = false;
}

void Drive::set() {
  if (_enable) {
  left.set();
  right.set();
  }
}

void Drive::power(int data) {
  _power = data;
  right.value((_power*_rightMax) >> 8);
  left.value((_power*_leftMax) >> 8);
  Drive::set();   
}

void Drive::steer(int data) {
  _steer = data;
  if (_steer < 0) {
    left.value(((256+_steer)*_power*_leftMax) >> 16);  
    right.value((_power*_rightMax) >> 8);
  }
  else {
    right.value(((256-_steer)*_power*_rightMax) >> 16);  
    left.value((_power*_leftMax) >> 8);
  }
  Drive::set(); 
}

void Drive::forward() {
  right.value((_power*_rightMax) >> 8);
  left.value((_power*_leftMax) >> 8); 
  Drive::set();
}

void Drive::stop() {
  right.stop();
  left.stop();  
}

void Drive::hardRight() {
  right.stop();
  left.fullSpeed();  
}

void Drive::hardLeft() {
  left.stop();
  right.fullSpeed();  
}

void Drive::startAutonomous() {
   Drive::enable();
  _auto = true;
}


void Drive::stopAutonomous() {
   Drive::disable();
  _auto = false;
  Drive::stop();
}

void Drive::turnRight() {
  Drive::hardRight();
  _start = millis()+_delayRight;
}

void Drive::turnLeft() {
  Drive::hardLeft();
  _start = millis()+_delayLeft;  
}

void Drive::turn180() {
  _start = millis()+_delayRight; 
  Drive::hardRight(); 
}

int Drive::delayLeft(){
      return _delayLeft;
}

int Drive::delayRight(){
      return _delayRight;
}

int Drive::delay180(){
      return _delay180;
}

void Drive::delayLeft(int value){
      _delayLeft = value;
}

void Drive::delayRight(int value){
      _delayRight = value;
}

void Drive::delay180(int value){
      _delay180 = value;
}

void Drive::autonomous() {
  if (_auto) {
    if ((millis() > _start)) {
          Drive::forward();
          _start = MAXTIME;
    }
  }         
}

bool Drive::auto_en() {
  return _auto;
}

void Drive::_loadConfig() {
  File configFile = SPIFFS.open(_filename, "r");
  if (configFile) {
    Serial.println("Loading config file: " + _filename);
    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    configFile.close();

    StaticJsonBuffer<SENSORDATA_JSON_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(buf.get());
  
    if (root.success()) {
      root.prettyPrintTo(Serial);
      Serial.println();

      _leftMax = root["leftMax"];
      _rightMax = root["rightMax"];
      _delayLeft = root["delayLeft"];
      _delayRight = root["delayRight"];
      _delay180 = root["delay180"];
    }
  }
  else {
    Serial.println("Creating new config file: " + _filename);
    Drive::_saveConfig();
  }
}

void Drive::_saveConfig() {
  StaticJsonBuffer<SENSORDATA_JSON_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["leftMax"] = _leftMax;
  root["rightMax"] = _rightMax;
  root["delayLeft"] = _delayLeft;
  root["delayRight"] = _delayRight;
  root["delay180"] = _delay180;

  Serial.print("Writing to file: ");
  Serial.println(_filename);
  root.prettyPrintTo(Serial);
  Serial.println();
  
  File configFile = SPIFFS.open(_filename, "w");
  if (configFile) {
    root.printTo(configFile);
    configFile.close();
  }
  else Serial.println("Failed to open config file for writing");
}
  
