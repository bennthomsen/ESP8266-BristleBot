/* Main BristleBot Code */

#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))
#define S Serial

//#define Sprintln(a)
//#define Sprint(a)

//---- Pin definitions ------

#include "Motor.h"
#include "Drive.h"
#include "parameters.h"

Drive drive = Drive(LEFT,RIGHT);

#include "FS.h"
#include "Config.h"
#include "ProximityFunctions.h"
#include "WebServer.h"
#include "WSS.h"




void initialisePins(void) {
    pinMode(RIGHT,OUTPUT);
   // pinMode(LEFT,OUTPUT);
    pinMode(REDLED,OUTPUT);
    pinMode(BLUELED,OUTPUT);
    pinMode(IRTX,OUTPUT);
    pinMode(IRTXBACK,OUTPUT);
    pinMode(REDLEDBACK,OUTPUT);
    
    digitalWrite(RIGHT, LOW);
   // digitalWrite(LEFT, LOW);
    digitalWrite(IRTX, HIGH);         // Turn front IR Tx off
    digitalWrite(IRTXBACK, HIGH);     // Turn back IR Tx off
    digitalWrite(REDLED, HIGH);
    digitalWrite(REDLEDBACK, HIGH);
    digitalWrite(BLUELED, LOW);

    pinMode(IRRXL, INPUT_PULLUP);
    
}

void heartbeat(int pin, unsigned long current) {

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
  ssid[11] = mac.charAt(12);
  ssid[12] = mac.charAt(13);
  ssid[13] = mac.charAt(15);
  ssid[14] = mac.charAt(16);
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
  loadConfig();
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

    if (autonomous) {
      if (now || millis() > start) {
        now = false;
        switch (action) {
          case 0: {
            drive.stop();
          }
            break;
          case 1: {
            
          }
            break;
          case 2: {
            
          }
            break;
          case 3: {
            
          }
            break;
          case 4: {
            drive.resume();
          }
            break;
        }
      }
    }
}



