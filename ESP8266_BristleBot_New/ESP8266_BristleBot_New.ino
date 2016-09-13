/* Main BristleBot Code */

#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))

//---- Pin definitions ------

#define RIGHT 15    // Corresponds to GPI0_5 labelled pin D1 on NodeMCU board
#define LEFT 5    // Corresponds to GPIO_15 labelled pin D8 on NodeMCU board
#define REDLED 16     // Corresponds to GPIO16 labelled pin D0 on NodeMCU board this pin is also connected to the LED cathode on the NodeMCU board
#define IRTX 0    // Corresponds to GPIO0 labelled pin D3 on NodeMCU board
#define IRTXBACK 4    // Corresponds to GPIO4 labelled pin D2 on NodeMCU board
#define BLUELED 2    // Corresponds to GPIO2 labelled pin D4 on NodeMCU board
#define REDLEDBACK 10    // Corresponds to GPIO10 labelled pin SD3 on NodeMCU board

#define IRRXL 12    // Corresponds to GPIO12 labelled pin D6 on NodeMCU board
#define IRRXR 14    // Corresponds to GPIO14 labelled pin D5 on NodeMCU board


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

char *ssid = "BristleBot_00";
const char *password = "uclbristlebot";
int power = 0;
int steer = 0;
int leftmotor = 255;
int rightmotor = 255;
unsigned long motorleft = 0;
unsigned long motorright = 0;

float leftProxSlope = 0;
float rightProxSlope = 0;
float leftProxOffset = 0;
float rightProxOffset = 0;

volatile int rightThreshold = 150;
volatile int leftThreshold = 150;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

boolean heartbeat_enable = true;
unsigned long previous = 0;
int blinkTime = 100;
boolean ledstate = false;

#include "ProximityFunctions.h"
#include "WebServer.h"
#include "WSS.h"



void initialisePins(void) {
    pinMode(RIGHT,OUTPUT);
    pinMode(LEFT,OUTPUT);
    pinMode(REDLED,OUTPUT);
    pinMode(BLUELED,OUTPUT);
    pinMode(IRTX,OUTPUT);
    pinMode(IRTXBACK,OUTPUT);
    pinMode(REDLEDBACK,OUTPUT);
    
    digitalWrite(RIGHT, LOW);
    digitalWrite(LEFT, LOW);
    digitalWrite(IRTX, HIGH);         // Turn front IR Tx off
    digitalWrite(IRTXBACK, HIGH);     // Turn back IR Tx off
    digitalWrite(REDLED, HIGH);
    digitalWrite(REDLEDBACK, HIGH);
    digitalWrite(BLUELED, LOW);

    pinMode(IRRXL, INPUT_PULLUP);
    analogWriteFreq(400);             // Set frequency for PWM
}

void heartbeat(int pin, unsigned long current){

  if (!heartbeat_enable) return;
  else {
    if(current - previous >= blinkTime) {
      previous = current;
      if (ledstate) {
        // Turn LED Off
        digitalWrite(pin, HIGH);
        ledstate = false;
        blinkTime = 900;
      }
      else {
        // Turn LED On
        digitalWrite(pin, LOW);
        ledstate = true;
        blinkTime = 100;
      }
    } 
  }
}



void setup() {
  Serial.begin(115200);
  initialisePins();
      
/* Start File System      */
  bool ok = SPIFFS.begin();
  if (ok) Serial.println ( "File system OK" ) ;
  else Serial.println ( "Warning: File System did not initialise" ) ;

/* Create Access point on ESP8266     */ 
  WiFi.mode(WIFI_AP);
  
  String mac = WiFi.macAddress();
  Sprintln("MAC address: " + mac);
  ssid[11] = mac.charAt(15);
  ssid[12] = mac.charAt(16);
  Sprint("SSID: ");
  Sprintln(ssid);
  Sprint("Password: ");
  Sprintln(password);
  
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Sprintln("AP IP address: " + myIP);


/* Start the HTTP server      */
  server.onNotFound(handleNotFound);
  server.begin();
  Sprintln ( "HTTP server started" );

 /* Start the Web Socket server      */ 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Sprintln ( "Web Socket server started" );
}

void loop() {
    server.handleClient();        // Check for WebServer requests
    webSocket.loop();             // Check for WebSocket requests


    currentMillis = millis();
    heartbeat(REDLED,currentMillis);
    if (prox_sensor_run) {
      
      if(currentMillis - previousMillis >= proximityInterval) {
        previousMillis = currentMillis;
        acquireProximity();
      }
    }
    sendProximity();           // Send updated Proximity reading if available
}


