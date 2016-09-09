#ifndef WebServer_h
#define WebServer_h

#include <Arduino.h>
#include "FS.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <Hash.h>
#include <ESP8266WebServer.h>

#define PROXIMITY_INTERVAL 1000
WebSocketsServer webSocket = WebSocketsServer(81);

ESP8266WebServer server(80);


void servePage(String file) {
  bool exist = SPIFFS.exists(file);
  if (exist) {
    Serial.println("The file exists");
    File f = SPIFFS.open(file, "r");
      if(!f){
        Serial.println(file + " failed to open");
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

void serveCSS(String file){
  bool exist = SPIFFS.exists(file);
  if (exist) {
    Serial.println("The image exists");
    File dataFile = SPIFFS.open(file, "r");
      if(!dataFile){
        Serial.println(file + " failed to open");
      }
      else {
        if (server.streamFile(dataFile, "text/css") != dataFile.size()) {}
        dataFile.close();
      }
  }
  else {
    Serial.println("No such file found.");
  }
}

void serveJS(String file){
  bool exist = SPIFFS.exists(file);
  if (exist) {
    Serial.println("The image exists");
    File dataFile = SPIFFS.open(file, "r");
      if(!dataFile){
        Serial.println(file + " failed to open");
      }
      else {
        if (server.streamFile(dataFile, "application/javascript") != dataFile.size()) {}
        dataFile.close();
      }
  }
  else {
    Serial.println("No such file found.");
  }
}

void serveImage(String file){
  bool exist = SPIFFS.exists(file);
  if (exist) {
    Serial.println("The image exists");
    File dataFile = SPIFFS.open(file, "r");
      if(!dataFile){
        Serial.println(file + " failed to open");
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

