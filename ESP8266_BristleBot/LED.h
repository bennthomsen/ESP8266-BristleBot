#ifndef LED_h
#define LED_h

#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>

#define SENSORDATA_JSON_SIZE (JSON_OBJECT_SIZE(6))

class LED
{
  public:
    LED();
    void begin(char* name, int pin, bool active);
    void on();
    void off();
    void flash();
    void blinkOn();
    void blinkOff();
    void heartbeat();
    void setOnTime(int onTime);
    void setOffTime(int offTime);
    void setDuty(float duty);
    void setPeriod(int period);
    void state(int value);
    int state();
    void mod38k(int cycles,int level);
    void updateConfig(char *data);
    void configJSON(char* outString, int size);

  private:
    int _pin;
    bool _active = true;
    int _period = 1000;
    int _onTime = 10;
    int _offTime = 990;
    int _rate = _onTime;
    float _duty = 0.01;
    unsigned long _previous = 0;
    bool _led;
    int _state = 2;
    String _filename;

    void _on();
    void _off();
    void _loadConfig();
    void _saveConfig();
    void _onOffTime();
};

#endif
