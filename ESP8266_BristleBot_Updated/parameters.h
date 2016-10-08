
#define RIGHT 15    // Corresponds to GPI0_5 labelled pin D1 on NodeMCU board
#define LEFT 5    // Corresponds to GPIO_15 labelled pin D8 on NodeMCU board
#define REDLED 16     // Corresponds to GPIO16 labelled pin D0 on NodeMCU board this pin is also connected to the LED cathode on the NodeMCU board
#define IRTX 4    // Corresponds to GPIO0 labelled pin D2 on NodeMCU board
#define IRTXBACK 0    // Corresponds to GPIO4 labelled pin D3 on NodeMCU board
#define BLUELED 2    // Corresponds to GPIO2 labelled pin D4 on NodeMCU board
#define REDLEDBACK 10    // Corresponds to GPIO10 labelled pin SD3 on NodeMCU board

#define IRRXL 12    // Corresponds to GPIO14 labelled pin D5 on NodeMCU board
#define IRRXR 14    // Corresponds to GPIO12 labelled pin D6 on NodeMCU board


long proximityInterval = 1000;

//int oldL = 0;
//int oldR = 0;
int front = 1;
//int frontdet = 0;
//int Ldetect = 0;
//int Rdetect = 0;
//String distance;
boolean prox_sensor_run = false;
boolean newSampleAvailable = false;

char *ssid = "BristleBot_0000";
const char *password = "uclbristlebot";

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

boolean heartbeat_enable;
unsigned long previous = 0;
int blinkTime = 100;
boolean ledstate = false;

#define STOP 0
#define FORWARD 1
#define TURNRIGHT 2
#define TURNLEFT 3
#define RESUMEOPERATION 4

boolean autonomous = false;
boolean now = false;

int duration = 3000;
int action = 1;

unsigned long start = 0;

unsigned int state;
uint8_t socketNumber;

//Motor left = Motor(LEFT);
//Motor right = Motor(RIGHT);



struct Proximity {
  long rate;
  int value;
  int threshold;
  bool limit;
  bool done;
};

struct Sensors {
  struct Proximity left;
  struct Proximity right;
};

Sensors sensors = {1000,0,270,false,false,0,270,false,false};
