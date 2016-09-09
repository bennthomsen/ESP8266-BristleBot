#ifndef ProximityFunctions_h
#define ProximityFunctions_h

#include "WebSocketServer.h"

//----------------------------------------------------------------------- 

void leftProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXL));
  pulselengthL = millis() - LeftStart;
  Ldetect = 1;
  frontdet = front;
}

void rightProximity() {
  detachInterrupt(digitalPinToInterrupt(IRRXR));
  pulselengthR = millis() - RightStart;
  Rdetect = 1;
  frontdet = front;
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
  Ldetect = 0;
  Rdetect = 0;
  attachInterrupt(digitalPinToInterrupt(IRRXL), leftProximityStart, FALLING);
  attachInterrupt(digitalPinToInterrupt(IRRXR), rightProximityStart, FALLING);
   for (int i=0; i <= cycles; i++){
        digitalWrite(pin, LOW);
        delayMicroseconds(12);
        digitalWrite(pin, HIGH);
        delayMicroseconds(12);
      }
}

#endif
