#include <Arduino.h>
#include "FS.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WebServer.h>

#define USE_SERIAL Serial

#define REDLED 16     // Corresponds to GPIO16 labelled pin D0 on NodeMCU board this pin is also connected to the LED cathode on the NodeMCU board
#define IRTX 0    // Corresponds to GPIO0 labelled pin D3 on NodeMCU board
#define IRTXBACK 4    // Corresponds to GPIO4 labelled pin D2 on NodeMCU board
#define BLUELED 2    // Corresponds to GPIO2 labelled pin D4 on NodeMCU board
#define REDLEDBACK 10    // Corresponds to GPIO10 labelled pin SD3 on NodeMCU board

#define IRRXR 12    // Corresponds to GPIO12 labelled pin D6 on NodeMCU board
#define IRRXL 14    // Corresponds to GPIO14 labelled pin D5 on NodeMCU board

#define PROXIMITY_INTERVAL 1000
WebSocketsServer webSocket = WebSocketsServer(81);
const char *ssid = "ESPap";
const char *password = "thereisnospoon";
// Create an instance of the server specify the port to listen on as an argument

ESP8266WebServer server(80);

unsigned long previousMillis = 0;
unsigned long startMicros = 0;
unsigned long LeftStart = 0;
unsigned long RightStart = 0;
const long interval = 1000;
volatile int pulselengthL = 0;
volatile int pulselengthR = 0;
int oldL = 0;
int oldR = 0;
int front = 1;
int frontdet = 0;
int Ldetect = 0;
int Rdetect = 0;
String distance;

uint8_t remote_ip;
uint8_t socketNumber;

 volatile int rightThreshold = 150;
 volatile int leftThreshold = 150;

//----------------------------------------------------------------------- 

void handleWebsite(){
 bool exist = SPIFFS.exists("/prox_sensor.html");
  if (exist) {
    Serial.println("The file exists");
    File f = SPIFFS.open("/prox_sensor.html", "r");
      if(!f){
        Serial.println("/prox_sensor.html failed to open");
      }
      else {
        String data = f.readString() ;
        server.send(200,"text/html",data);
        f.close();
      }
  }
  else {
    Serial.println("No such file found.");
  }
  
} 

//----------------------------------------------------------------------- 

void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for ( uint8_t i = 0; i < server.args(); i++ ) {
        message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
    }
    server.send ( 404, "text/plain", message );
}
//----------------------------------------------------------------------- 

// state machine states
unsigned int state;
#define SEQUENCE_IDLE 0x00
#define GET_SAMPLE 0x10
#define GET_SAMPLE__WAITING 0x12

void proximityRead(void){
if (state == SEQUENCE_IDLE){
  return;
  }
else if (state == GET_SAMPLE){
  state = GET_SAMPLE__WAITING;
  return;
  }
else if (state == GET_SAMPLE__WAITING){
   String prox_L = String (pulselengthL);
   String prox_R = String (pulselengthR);

  webSocket.sendTXT(socketNumber , "{\"left\":" + prox_L + "}");
  webSocket.sendTXT(socketNumber , "{\"right\":" + prox_R + "}");
 
  //delay(500);

  return;
  }
}
//----------------------------------------------------------------------- 

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  IPAddress ip = webSocket.remoteIP(num);  
  socketNumber = num;
  state = GET_SAMPLE;
  
}

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

//----------------------------------------------------------------------- 

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

    pinMode(IRRXL, INPUT_PULLUP);
    analogWriteFreq(400);

/* Start File System      */
  bool ok = SPIFFS.begin();
  if (ok) Serial.println ( "File system OK" ) ;
  else Serial.println ( "Warning: File System did not initialise" ) ;

/* Create Access point on ESP8266     */ 
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  USE_SERIAL.print("AP IP address: ");
  USE_SERIAL.println(myIP);

/* Start the HTTP server      */
  server.on("/",handleWebsite);
  server.on ( "/inline", []() {server.send ( 200, "text/plain", "this works as well" );} );
  server.onNotFound ( handleNotFound );
  server.begin();
  USE_SERIAL.println ( "HTTP server started" );

 /* Start the Web Socket server      */ 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  USE_SERIAL.println ( "Web Socket server started" );
}

void loop() {
    server.handleClient();
    proximityRead();
    webSocket.loop();

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
      
        distance = "Front Dist. L: " + String(pulselengthL) + " R:" + String(pulselengthR);
        USE_SERIAL.println(distance);
        
     }
     
}
