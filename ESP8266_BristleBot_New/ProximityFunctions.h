#ifndef ProximityFunctions_h
#define ProximityFunctions_h

unsigned long lastSense;

struct IRSensor {
  int value;
  int threshold;
  long start;
  bool limit;
  bool done;
};

struct Proximity {
  bool enable;
  bool front;
  long rate;
  int cycles;
  int level;
  struct IRSensor left;
  struct IRSensor right;
};

Proximity sensors = {false,true,1000,7500,1,0,120,0,false,false,0,120,0,false,false};

// Function prototypes
void initialiseProximity();
void leftProximity();
void rightProximity();
void leftProximityStart();
void leftProximityStart();
void sensorInterrupts();
void acquireProximity();

void initialiseProximity() {
    pinMode(IRRXL, INPUT_PULLUP);
    pinMode(IRRXR, INPUT_PULLUP);
}

void leftProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXL));
  sensors.left.value = millis() - sensors.left.start;
  sensors.right.limit = (sensors.right.value > sensors.right.threshold);
  sensors.left.done = true;
}

void rightProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXR));
  sensors.right.value = millis() - sensors.right.start;
  sensors.right.limit = (sensors.right.value > sensors.right.threshold);
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
        blue.on();
        sensorInterrupts();
        ir.mod38k(sensors.cycles);
        blue.off();
      }
      else {
        ir.off();
        red.on();
        sensorInterrupts();
        irRear.mod38k(sensors.cycles);
        red.off();
      }
    }
  }
}

#endif
