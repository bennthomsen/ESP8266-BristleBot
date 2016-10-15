#ifndef ProximityFunctions_h
#define ProximityFunctions_h

#include <ArduinoJson.h>

#define SENSORCONFIG_JSON_SIZE (JSON_OBJECT_SIZE(8))

unsigned long lastSense;

struct IRSensor {
  int value;
  int threshold;
  long start;
  bool limit;
  bool done;
};

struct Proximity {
  volatile bool enable;
  bool front;
  long rate;            // sensor sampling rate
  long refreshRate;          // rate at which the sensor streams data
  int cycles;
  int level;
  struct IRSensor left;
  struct IRSensor right;
};

Proximity sensors = {false,true,500,2000,7500,1,0,120,0,false,false,0,120,0,false,false};

// Function prototypes
void initialiseProximity();
void proximityEnable(int val);
void leftProximity();
void rightProximity();
void leftProximityStart();
void leftProximityStart();
void sensorInterrupts();
void acquireProximity();
void proxUpdateConfig(char *data);
void proxConfigJSON(char* outString, int size);

void initialiseProximity() {
    pinMode(IRRXL, INPUT_PULLUP);
    pinMode(IRRXR, INPUT_PULLUP);
}

void proximityEnable(int val) {
  if (val) {
    sensors.enable= true;
  }
  else {
    sensors.enable= false;
  }
}

void leftProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXL));
  sensors.left.value = millis() - sensors.left.start;
  sensors.left.done = true;
}

void rightProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXR));
  sensors.right.value = millis() - sensors.right.start;
  sensors.right.done = true;
}

void leftProximityStart() {
  detachInterrupt(digitalPinToInterrupt(IRRXL));
  sensors.left.start = millis();
  attachInterrupt(digitalPinToInterrupt(IRRXL), leftProximity, RISING);
}

void rightProximityStart() {
  detachInterrupt(digitalPinToInterrupt(IRRXR));
  sensors.right.start= millis();
  attachInterrupt(digitalPinToInterrupt(IRRXR),  rightProximity, RISING);
}


void sensorInterrupts() {
  attachInterrupt(digitalPinToInterrupt(IRRXL), leftProximityStart, FALLING);
  attachInterrupt(digitalPinToInterrupt(IRRXR), rightProximityStart, FALLING);
}

void acquireProximity() {
  if (sensors.enable) {
    if(millis() - lastSense >= sensors.rate) {
      lastSense = millis();
      if (sensors.front) {
        ir.off();
        irRear.off();
        sensorInterrupts();
        ir.mod38k(sensors.cycles,sensors.level);
      }
      else {
        ir.off();
        irRear.off();
        sensorInterrupts();
        irRear.mod38k(sensors.cycles,sensors.level);
      }
    }
  }
}

void proxConfigJSON(char* outString, int size) {
  StaticJsonBuffer<SENSORCONFIG_JSON_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonObject& thresh = root.createNestedObject("threshold");
  root["enable"] = sensors.enable;
  root["rate"] = sensors.rate;
  root["refresh"] = sensors.refreshRate;
  root["cycles"] = sensors.cycles;
  root["level"] = sensors.level;
  thresh["left"] = sensors.left.threshold;
  thresh["right"] = sensors.right.threshold;

  root.printTo(outString,size);
  Serial.println(outString);
}

void proxUpdateConfig(char *data) {
  StaticJsonBuffer<SENSORCONFIG_JSON_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(data);
  
  if (root.success()) {
    if(root["rate"]) sensors.rate = root["rate"];
    if(root["refresh"]) sensors.refreshRate = root["refresh"];
    if(root["cycles"]) sensors.cycles = root["cycles"];
    if(root["level"]) sensors.level = root["level"];
    if(root["threshold"]["left"]) sensors.left.threshold = root["threshold"]["left"];
    if(root["threshold"]["right"]) sensors.right.threshold = root["threshold"]["right"];
  }
    else Serial.println("Failed to parse JSON"); 
}

#endif
