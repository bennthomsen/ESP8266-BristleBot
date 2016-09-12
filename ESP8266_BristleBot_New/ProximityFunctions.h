#ifndef ProximityFunctions_h
#define ProximityFunctions_h

volatile int pulselengthL = 0;
volatile int pulselengthR = 0;

volatile unsigned long LeftStart = 0;
volatile unsigned long RightStart = 0;

volatile boolean LeftAvailable = false;
volatile boolean RightAvailable = false;

void leftProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXL));
  pulselengthL = millis() - LeftStart;
  LeftAvailable = true;
   Sprint("LP ");
  Sprintln(pulselengthR);
  //frontdet = front;
}

void rightProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXR));
  pulselengthR = millis() - RightStart;
  RightAvailable = true;
  Sprint("RP ");
  Sprintln(pulselengthR);
  
  //frontdet = front;
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
  attachInterrupt(digitalPinToInterrupt(IRRXL), leftProximityStart, FALLING);
  attachInterrupt(digitalPinToInterrupt(IRRXR), rightProximityStart, FALLING);
   for (int i=0; i <= cycles; i++){
        digitalWrite(pin, LOW);
        delayMicroseconds(12);
        digitalWrite(pin, HIGH);
        delayMicroseconds(12);
      }
}

float ProximityCal(int raw, float slope, float offset) {
  float distance = slope*float(raw) + offset;
  return distance;
}

void acquireProximity(void) {
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
