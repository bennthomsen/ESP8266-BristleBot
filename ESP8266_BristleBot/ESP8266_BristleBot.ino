#include "WebSocketServer.h"
#include "ProximityFunctions.h"

const char *ssid = "ESPap";
const char *password = "thereisnospoon";
// Create an instance of the server specify the port to listen on as an argument


//----------------------------------------------------------------------- 

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
          break;
        case WStype_CONNECTED: {
          IPAddress ip = webSocket.remoteIP(num);  
          socketNumber = num;
          state = GET_SAMPLE;
          }
          break;
        case WStype_TEXT:{
            String text = String((char *) &payload[0]);
            
          if(text=="LED") {
            digitalWrite(REDLED,LOW);
            delay(500);
            digitalWrite(REDLED,HIGH);
            USE_SERIAL.println("led just lit");
            String reply = "{\"led\":\"ON\"}";
            webSocket.sendTXT(num, reply);
           }

          if(text=="BATT") {
            int adc = analogRead(A0);
            int voltage = (5222*adc) >> 10;
            String voltageData = "Supply Voltage: " + String(voltage) +"mV";
            USE_SERIAL.println(voltageData);
            String reply = "{\"battery\":";
            reply += voltage;
            reply += "}";
            webSocket.sendTXT(num, reply);

           }
           
          if(text.startsWith("s")) {
              String xVal=(text.substring(text.indexOf("s")+1,text.length())); 
              steer = xVal.toInt();
              analogWrite(RIGHT,constrain(power*steer >> 8, 0, 255));
              analogWrite(LEFT,constrain(power*(255-steer) >> 8, 0, 255));
           }


           if(text.startsWith("p")){
            String yVal=(text.substring(text.indexOf("p")+1,text.length())); 
            power = yVal.toInt();
            analogWrite(RIGHT,constrain(power*steer >> 8, 0, 255));
            analogWrite(LEFT,constrain(power*(255-steer) >> 8, 0, 255));
           }

           if(text=="RESET"){
             analogWrite(RIGHT,LOW);
             analogWrite(LEFT,LOW);
             power = 0;
             USE_SERIAL.println("reset");
            }

            if(text.startsWith("r")){
              String val=(text.substring(text.indexOf("r")+1,text.length())); 
              rightThreshold = val.toInt();
              USE_SERIAL.print("Right Threshold: " + val);
            }    

            if(text.startsWith("l")){
              String val=(text.substring(text.indexOf("l")+1,text.length())); 
              leftThreshold = val.toInt();
              USE_SERIAL.print("Left Threshold: " + val);
            }  
         

          if(text=="mainpage") {
            Serial.println("start.html request registered");
            handleStart();
          }

          if(text=="proxweb") {
            Serial.println("prox_sensor.html request registered");
            prox_sensor_run = true;
            handleProx();
          }

          if(text=="websock") {
            Serial.println("websocketserver.html request registered");
            handleWeb();
          }

          if(text=="acceler") {
            Serial.println("accelerometer_mozilla.html request registered");
            handleAccel();
          }
        }
        break;
        
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
    pinMode(IRTX,OUTPUT);
    pinMode(IRTXBACK,OUTPUT);
    pinMode(REDLEDBACK,OUTPUT);
    
    digitalWrite(RIGHT, LOW);
    digitalWrite(LEFT, LOW);
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
  server.on("/logo.png",loadImage);
  server.on("/start.html",handleStart);
  server.on("/prox_sensor.html",handleProx);
  server.on("/websocketserver.html",handleWeb);
  server.on("/accelerometer_mozilla.html",handleAccel);
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
        //USE_SERIAL.println(distance);
        
     }
     
}

