#ifndef ProximityFunctions_h
#define ProximityFunctions_h

volatile int pulselengthL = 0;
volatile int pulselengthR = 0;

volatile unsigned long LeftStart = 0;
volatile unsigned long RightStart = 0;

volatile boolean LeftAvailable = false;
volatile boolean RightAvailable = false;

boolean useProximity = false;


void leftProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXL));
  sensors.left.value = millis() - LeftStart;
  if (sensors.left.value > sensors.left.threshold) {
    sensors.left.limit = true;
  }
  sensors.left.done = true;
  Sprint("LP ");
  Sprintln(sensors.left.value);
}

void rightProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXR));
  sensors.right.value = millis() - RightStart;
  if (sensors.right.value > sensors.right.threshold) {
    sensors.right.limit = true;
  }
  sensors.right.done = true;
  Sprint("LP ");
  Sprintln(sensors.right.value);
}

void leftProximityStart() {
  detachInterrupt(digitalPinToInterrupt(IRRXL));
  LeftStart = millis();
  attachInterrupt(digitalPinToInterrupt(IRRXL), leftProximity, RISING);
}

void rightProximityStart() {
  detachInterrupt(digitalPinToInterrupt(IRRXR));
  RightStart = millis();
  attachInterrupt(digitalPinToInterrupt(IRRXR),  rightProximity, RISING);
}


void IRmod(char pin, int cycles) {
  attachInterrupt(digitalPinToInterrupt(IRRXL), rightProximityStart, FALLING);
  attachInterrupt(digitalPinToInterrupt(IRRXR), rightProximityStart, FALLING);
   for (int i=0; i <= cycles; i++){
        digitalWrite(pin, LOW);
        delayMicroseconds(1);
        digitalWrite(pin, HIGH);
        delayMicroseconds(23);
      }
}

void acquireProximity(void) {
        //analogWrite(RIGHT,0);
        //analogWrite(LEFT,0);
        if (front) {
          digitalWrite(BLUELED, LOW);
          IRmod(IRTX, 10000); 
          digitalWrite(BLUELED, HIGH);
        }
        else {
          digitalWrite(REDLEDBACK, LOW);
          IRmod(IRTXBACK, 10000);
          digitalWrite(REDLEDBACK, HIGH);
        }
}


#endif
