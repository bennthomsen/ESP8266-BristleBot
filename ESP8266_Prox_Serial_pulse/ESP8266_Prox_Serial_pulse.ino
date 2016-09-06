/*
BristleBot proximity sensing
*/

//pin assignments
#define IRTX 0    // Corresponds to GPIO0 labelled pin D3 on NodeMCU board
#define IRTXBACK 4    // Corresponds to GPIO4 labelled pin D2 on NodeMCU board
#define BLUELED 2    // Corresponds to GPIO2 labelled pin D4 on NodeMCU board
#define REDLEDBACK 10    // Corresponds to GPIO10 labelled pin SD3 on NodeMCU board
#define REDLED 16     // Corresponds to GPIO16 labelled pin D0 on NodeMCU board this pin is also connected to the LED cathode on the NodeMCU board

#define IRRXR 12    // Corresponds to GPIO12 labelled pin D6 on NodeMCU board
#define IRRXL 14    // Corresponds to GPIO14 labelled pin D5 on NodeMCU board

#define PROXIMITY_INTERVAL 1000

#define USE_SERIAL Serial

unsigned long previousMillis = 0;
unsigned long startMicros = 0;
unsigned long LeftStart = 0;
unsigned long RightStart = 0;
const long interval = 1000;
volatile int pulselengthL = 0;
volatile int pulselengthR = 0;

volatile int pulselengthLL = 0;
volatile int pulselengthRR = 0;

int oldL = 0;
int oldR = 0;
int steer = 128;
int power = 0;
int front = 1;
int frontdet = 0;
int Ldetect = 0;
int Rdetect = 0;
 String distance;

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

void setup() {
   
    USE_SERIAL.begin(115200);

    pinMode(REDLED,OUTPUT);
    pinMode(BLUELED,OUTPUT);
    pinMode(IRTX,OUTPUT);
    pinMode(IRTXBACK,OUTPUT);
    pinMode(REDLEDBACK,OUTPUT);
    
    digitalWrite(IRTX, LOW);
    digitalWrite(IRTXBACK, LOW);
    digitalWrite(REDLED, HIGH);
    digitalWrite(REDLEDBACK, HIGH);
    digitalWrite(BLUELED, LOW);

    //pinMode(IRRXL, INPUT_PULLUP);
    pinMode(IRRXL, INPUT);
    pinMode(IRRXR, INPUT);
    analogWriteFreq(400);
}
unsigned long durationL;
unsigned long durationR;
void loop() {

    // pulse width measurement proximity sensor
    durationL = pulseIn(IRRXL, HIGH);
    durationR = pulseIn(IRRXR, HIGH);

    USE_SERIAL.println("Left: " + (String)durationL + "; Right: " + (String)durationR);

    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= PROXIMITY_INTERVAL) {
      previousMillis = currentMillis;
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

     if (frontdet ) {

      //mapping
        pulselengthLL = (-0.1403*pulselengthL) + 35.409;
        pulselengthRR = (-0.1403*pulselengthR) +35.409;
        
       // distance = "Dist. L: " + String(pulselengthLL) + " cm" + " R:" + String(pulselengthRR) + " cm";
        distance = "Dist. L: " + String(pulselengthL) + " R:" + String(pulselengthR);
        
        USE_SERIAL.println(distance);
     }
     
}


