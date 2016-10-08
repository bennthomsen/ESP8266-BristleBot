#include "Arduino.h"
#include "LED.h"

LED::LED() {}

void LED::begin(char* name, int pin, bool active) {
  _active = active;
  _pin = pin;
  pinMode(_pin, OUTPUT);
  _filename = "/" + String(name) + ".json";
  LED::_loadConfig();
  LED::state(_state);
}

void LED::state(int value) {
  _state = value;
  switch (_state) {
    case 0: LED::_off();
    break;
    case 1: LED::_on();
    break;
  }
}

int LED::state() {
  return _state;
}

void LED::_on() {
  digitalWrite(_pin,_active);
  _led = true;
}

void LED::_off() {
  digitalWrite(_pin,!(_active));
  _led = false;
}

void LED::on() {
  _state = 1;
  LED::_on();
  LED::_saveConfig();
}

void LED::off() {
  _state = 0;
  LED::_off();
  LED::_saveConfig();
}

void LED::flash() {
  LED::_on();
  delay(10);
  LED::_off();
}

void LED::setOnTime(int onTime) {
  _onTime = onTime;
  _offTime = _period - _onTime;
}

void LED::setPeriod(int period) {
  _period = period;
  _offTime = _period - _onTime;
}

void LED::setDuty(float duty) {
  _duty = duty;
  LED::_onOffTime();
}

void LED::_onOffTime() {
  _onTime = int(_duty*_period);
  _offTime = _period - _onTime;
}

void LED::setLevel(int level) {
  _level = level;
}

void LED::blinkOn() {
  _state = 2;
  LED::_saveConfig();
}

void LED::blinkOff() {
  _state = 0;
  LED::off();
  LED::_saveConfig();
}

void LED::mod38k(int cycles) {
    for (int i=0; i <= cycles; i++){
      digitalWrite(_pin, _active);
      delayMicroseconds(_level);
      digitalWrite(_pin, !(_active));
      delayMicroseconds(24-_level);
    }
}

void LED::heartbeat() {
  if (_state == 2) {
    if(millis() - _previous >= _rate) {
      _previous = millis();
      if (_led) {
        LED::_off();
        _rate = _offTime;
      }
      else {
        LED::_on();
        _rate = _onTime;
      }
    } 
  }
}

void LED::updateConfig(char *data) {
  StaticJsonBuffer<SENSORDATA_JSON_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(data);
  
  if (root.success()) {
//    root.prettyPrintTo(Serial);
//    Serial.println();
  
    _period = root["period"];
    _duty = root["duty"];
    _level = root["level"];
    LED::_onOffTime();
    LED::_saveConfig(); 
  }
    else Serial.println("Failed to parse JSON"); 
}

void LED::_saveConfig() {
  StaticJsonBuffer<SENSORDATA_JSON_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["state"] = _state;
  root["period"] = _period;
  root["duty"] = _duty;
  root["level"] = _level;

//  Serial.print("Writing to file: ");
//  Serial.println(_filename);
//  root.prettyPrintTo(Serial);
//  Serial.println();
  
  File configFile = SPIFFS.open(_filename, "w");
  if (configFile) {
    root.printTo(configFile);
    configFile.close();
  }
  else Serial.println("Failed to open config file for writing");
}

void LED::_loadConfig() {

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
//      root.prettyPrintTo(Serial);
//      Serial.println();

      _state = root["state"];
      _period = root["period"];
      _duty = root["duty"];
      _level = root["level"];

      LED::state(_state);
      LED::_onOffTime();
    }
  }
  else {
    Serial.println("Creating new config file: " + _filename);
    LED::_saveConfig();
  }
}

void LED::configJSON(char* outString, int size) {
  StaticJsonBuffer<SENSORDATA_JSON_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["state"] = _state;
  root["period"] = _period;
  root["duty"] = _duty;
  root["level"] = _level;

  root.printTo(outString,size);
}

