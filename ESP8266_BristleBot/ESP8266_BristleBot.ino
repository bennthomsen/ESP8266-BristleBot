/* BristleBot Web Controller
 *  BristleBot ESP8266 firmware for the the web control App.
 *  The ESP8266 is setup up as a WiFi Access point which you can connect to with any WiFi enabled
 *  device. 
 *  HTPP requests to 194.168.4.1 will serve the BristleBot App.
 *  
 *  The ESP8266 also runs a Web Socket Server taht handles the communication between the App and the BristleBot 
 *  
 *  Benn Thomsen, Oct 2016
*/

#include <Arduino.h>

/* Main BristleBot Code */

#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))

//#define Sprintln(a)
//#define Sprint(a)

//---- Pin definitions ------

#include "PinMapping.h"
#include "LED.h"
#include "Drive.h"
#include <WebSocketsServer.h>
#include <ArduinoJson.h>


WebSocketsServer webSocket = WebSocketsServer(81);
uint8_t socketNumber;

Drive drive = Drive(LEFT,RIGHT);
LED blue;
LED red;
LED ir;
LED irRear;


#include "ProximityFunctions.h"
#include "FS.h"
//#include "Config.h"
#include "WebServer.h"
#include "WSS.h"

void setup() {
  Serial.begin(115200);

/* Start File System      */
  if (SPIFFS.begin()) Serial.println ( "File system OK" ) ;
  else Serial.println ( "Warning: File System did not initialise" ) ;

/* Initialise LEDs     */   
  blue.begin("blue",BLUELED,false);       // setup active low blue LED
  red.begin("red",REDLED,false);         // setup active low red LED
  ir.begin("ir",IRTX,false);            // setup active low front ir LED
  irRear.begin("irrear",IRTXREAR,false);     // setup active low rear ir LED

/* Initialise Drive controller    */ 
  drive.begin("drive"); 

/* Create Access point on ESP8266 and start Web server    */ 
  configureAccessPoint();

 /* Start the Web Socket server      */ 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Sprintln ( "Web Socket server started" );
}

void loop() {
    server.handleClient();        // Check for WebServer requests
    webSocket.loop();             // Check for WebSocket requests
    red.heartbeat();              // Execute Red Heartbeat if enabled
    blue.heartbeat();             // Execute Blue Heartbeat if enabled
    ir.heartbeat();               // Execute IR Heartbeat if enabled
    drive.loop();                 // Service Drive operation
    acquireProximity();           // Start Proximity sensor reading if enabled
    sendProximity();              // Send updated Proximity sensor reading if available
}



