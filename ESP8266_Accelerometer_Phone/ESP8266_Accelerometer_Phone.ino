
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
#define BLUELED 2    // Corresponds to GPIO2 labelled pin D4 on NodeMCU board

#define IRRXR 12    // Corresponds to GPIO12 labelled pin D6 on NodeMCU board
#define IRRXL 14    // Corresponds to GPIO14 labelled pin D5 on NodeMCU board

#define PROXIMITY_INTERVAL 1000
WebSocketsServer webSocket = WebSocketsServer(81);
const char *ssid = "ESPap";
const char *password = "thereisnospoon";
// Create an instance of the server specify the port to listen on as an argument

ESP8266WebServer server(80);

int steer = 128;
int power = 0;

//----------------------------------------------------------------------- 
void handleWebsite(){
  bool exist = SPIFFS.exists("/accelerometer.html");
  if (exist) {
    Serial.println("The file exists");
    File f = SPIFFS.open("/accelerometer.html", "r");
      if(!f){
        Serial.println("/accelerometer.html failed to open");
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

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    IPAddress ip = webSocket.remoteIP(num);  
    switch(type) {
        case WStype_TEXT:{
            String text = String((char *) &payload[0]);
           
          if(text.startsWith("s")) {
              String xVal=(text.substring(text.indexOf("s")+1,text.length())); 
              steer = xVal.toInt();
              analogWrite(RIGHT,constrain(power*steer >> 8, 0, 255));
              analogWrite(LEFT,constrain(power*(255-steer) >> 8, 0, 255));

              USE_SERIAL.println("Steer: " + steer);
           }


           if(text.startsWith("p")){
            String yVal=(text.substring(text.indexOf("p")+1,text.length())); 
            power = yVal.toInt();
            analogWrite(RIGHT,constrain(power*steer >> 8, 0, 255));
            analogWrite(LEFT,constrain(power*(255-steer) >> 8, 0, 255));

            USE_SERIAL.println("Power: " + power);
           }
        break;
      }
        case WStype_BIN:
            hexdump(payload, length);
            // echo data back to browser
            webSocket.sendBIN(num, payload, length);
            break;
    }
}


void setup() {
   
    USE_SERIAL.begin(115200);
    pinMode(RIGHT,OUTPUT);
    pinMode(LEFT,OUTPUT);
    pinMode(REDLED,OUTPUT);
    pinMode(BLUELED,OUTPUT);
    
    digitalWrite(RIGHT, LOW);
    digitalWrite(LEFT, LOW);
    digitalWrite(REDLED, HIGH);
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
    webSocket.loop();
}

