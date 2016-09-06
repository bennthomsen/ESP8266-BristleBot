#ifndef WebSocketServer_h
#define WebSocketServer_h

#include <Arduino.h>
#include "FS.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WebServer.h>


#define USE_SERIAL Serial

#define RIGHT 15    // Corresponds to GPIO5 labelled pin D1 on NodeMCU board
#define LEFT 5    // Corresponds to GPIO15 labelled pin D8 on NodeMCU board
#define REDLED 16     // Corresponds to GPIO16 labelled pin D0 on NodeMCU board this pin is also connected to the LED cathode on the NodeMCU board
#define IRTX 0    // Corresponds to GPIO0 labelled pin D3 on NodeMCU board
#define IRTXBACK 4    // Corresponds to GPIO4 labelled pin D2 on NodeMCU board
#define BLUELED 2    // Corresponds to GPIO2 labelled pin D4 on NodeMCU board
#define REDLEDBACK 10    // Corresponds to GPIO10 labelled pin SD3 on NodeMCU board

#define IRRXR 12    // Corresponds to GPIO12 labelled pin D6 on NodeMCU board
#define IRRXL 14    // Corresponds to GPIO14 labelled pin D5 on NodeMCU board

#define PROXIMITY_INTERVAL 1000
WebSocketsServer webSocket = WebSocketsServer(81);

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
int steer = 128;
int power = 0;
int front = 1;
int frontdet = 0;
int Ldetect = 0;
int Rdetect = 0;
String distance;

 boolean prox_sensor_run = false;
 uint8_t socketNumber;

 volatile int rightThreshold = 150;
 volatile int leftThreshold = 150;

 // state machine states
unsigned int state;
#define SEQUENCE_IDLE 0x00
#define GET_SAMPLE 0x10
#define GET_SAMPLE__WAITING 0x12 

  //----------------------------------------------------------------------- 
void handleWebsite(){
  bool exist = SPIFFS.exists("/index.html");
  if (exist) {
    Serial.println("The file exists");
    File f = SPIFFS.open("/index.html", "r");
      if(!f){
        Serial.println("/index.html failed to open");
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

//----------------------------------------------------------------------
void handleStart(){
  bool exist = SPIFFS.exists("/start.html");
  if (exist) {
    Serial.println("The file exists");
    File f = SPIFFS.open("/start.html", "r");
      if(!f){
        Serial.println("/start.html failed to open");
      }
      else {
        String data = f.readString();
        server.send(200,"text/html",data);
        f.close();
      }
  }
  else {
    Serial.println("No such file found.");
  }
}

//----------------------------------------------------------------------- 
void handleProx(){
  bool exist = SPIFFS.exists("/prox_sensor.html");
  if (exist) {
    Serial.println("The file exists");
    File f = SPIFFS.open("/prox_sensor.html", "r");
      if(!f){
        Serial.println("/prox_sensor.html failed to open");
      }
      else {
        String data = f.readString();
        server.send(200,"text/html",data);
        f.close();
      }
  }
  else {
    Serial.println("No such file found.");
  }
}

//----------------------------------------------------------------------- 
void handleWeb(){
  bool exist = SPIFFS.exists("/websocketserver.html");
  if (exist) {
    Serial.println("The file exists");
    File f = SPIFFS.open("/websocketserver.html", "r");
      if(!f){
        Serial.println("/websocketserver.html failed to open");
      }
      else {
        String data = f.readString();
        server.send(200,"text/html",data);
        f.close();
      }
  }
  else {
    Serial.println("No such file found.");
  }
}

//----------------------------------------------------------------------- 

void handleAccel(){
  bool exist = SPIFFS.exists("/accelerometer_mozilla.html");
  if (exist) {
    Serial.println("The file exists");
    File f = SPIFFS.open("/accelerometer_mozilla.html", "r");
      if(!f){
        Serial.println("/accelerometer_mozilla.html failed to open");
      }
      else {
        String data = f.readString();
        server.send(200,"text/html",data);
        f.close();
      }
  }
  else {
    Serial.println("No such file found.");
  }
}

//----------------------------------------------------------------------- 
void loadImage(){
  bool exist = SPIFFS.exists("/logo.png");
  if (exist) {
    Serial.println("The image exists");
    File dataFile = SPIFFS.open("/logo.png", "r");
      if(!dataFile){
        Serial.println("/logo.png failed to open");
      }
      else {
        if (server.streamFile(dataFile, "image/png") != dataFile.size()) {}
        dataFile.close();
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
 
  delay(500);

  return;
  }
}

#endif
