#include <WebSocketsServer.h>
WebSocketsServer webSocket = WebSocketsServer(81);


#define SEQUENCE_IDLE 0x00
#define GET_SAMPLE 0x10
#define GET_SAMPLE__WAITING 0x12 

  //----------------------------------------------------------------------- 
void sendProximity() {
  if (sensors.left.done & sensors.right.done) {
    sensors.left.done = false;
    sensors.right.done = false;
    webSocket.sendTXT(socketNumber, "{\"left\":" + String(sensors.left.value) + ",\"right\":" + String(sensors.right.value) + "}");
  }
}

bool sendConfig(uint8_t num) {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  String configData = configFile.readString();
  Sprintln(configData);
  webSocket.sendTXT(num, configData);
}

  void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
          break;
        case WStype_CONNECTED: {
          IPAddress ip = webSocket.remoteIP(num);
          socketNumber = num; 
         }
          break;
        case WStype_TEXT: {
          String text = String((char *) &payload[0]);
            
          if(text=="RED_LED_BLINK") {
             heartbeat_enable = true;
            Sprintln("Red LED blink");
            saveConfig();
           }

          if(text=="RED_LED_ON") {
            heartbeat_enable = false;
            digitalWrite(REDLED,LOW);
            Sprintln("Red LED on");
            saveConfig();
           }

          if(text=="RED_LED_OFF") {
            heartbeat_enable = false;
            digitalWrite(REDLED,HIGH);
            Sprintln("Red LED off");
            saveConfig();
           }

          if(text=="BLUE_LED_BLINK") {
            digitalWrite(BLUELED,LOW);
            delay(500);
            digitalWrite(BLUELED,HIGH);
            Sprintln("BLUE LED blink");
           }

          if(text=="BLUE_LED_ON") {
            digitalWrite(BLUELED,LOW);
            Sprintln("BLUE LED on");
           }

          if(text=="BLUE_LED_OFF") {
            digitalWrite(BLUELED,HIGH);
            Sprintln("BLUE LED off");
           }

          if(text=="FRONT_LED_BLINK") {
            digitalWrite(IRTX,LOW);
            delay(500);
            digitalWrite(IRTX,HIGH);
            Sprintln("FRONT LED blink");
           }

          if(text=="FRONT_LED_ON") {
            digitalWrite(IRTX,LOW);
            Sprintln("FRONT LED on");
           }

          if(text=="FRONT_LED_OFF") {
            digitalWrite(IRTX,HIGH);
            Sprintln("FRONT LED off");
           }

          if(text=="BATT") {
            int adc = analogRead(A0);
            int voltage = (5222*adc) >> 10;
            String voltageData = "Supply Voltage: " + String(voltage) +"mV";
            Sprintln(voltageData);
            String reply = "{\"battery\":";
            reply += voltage;
            reply += "}";
            webSocket.sendTXT(num, reply);
           }

           if(text=="SETTINGS") {
            sendConfig(num);
           }

           if(text=="PROX_SINGLE") {
            acquireProximity();
            Sprintln("Proximity Sensor Single");
           }

           if(text=="PROX_EN") {
            prox_sensor_run = true;
            Sprintln("Run Proximity Sensor");
           }

           if(text=="PROX_DIS") {
            prox_sensor_run = false;
            Sprintln("Stop Proximity Sensor");
           }

           if(text=="AUTO_EN") {
            autonomous = true;
            Sprintln("Starting Autonomous operation");
           }

           if(text=="AUTO_DIS") {
            autonomous = false;
            Sprintln("Starting Autonomous operation");
           }
           
          if(text.startsWith("s")) {
            String xVal=(text.substring(text.indexOf("s")+1,text.length())); 
            drive.steer(xVal.toInt());
          }


           if(text.startsWith("p")){
            String yVal=(text.substring(text.indexOf("p")+1,text.length())); 
            drive.power(yVal.toInt());
           }

           if(text.startsWith("e")){
            String yVal=(text.substring(text.indexOf("e")+1,text.length()));
            drive.left.value(yVal.toInt());
           }

            if(text.startsWith("f")){
            String yVal=(text.substring(text.indexOf("f")+1,text.length()));
            drive.right.value(yVal.toInt());
           }

           if(text=="STOP"){
             drive.stop();
            }

            if(text.startsWith("r")){
              String val=(text.substring(text.indexOf("r")+1,text.length())); 
              drive.right.maximum(val.toInt());
              saveConfig();
              Sprintln("Right Threshold: " + val);
            }    

            if(text.startsWith("l")){
              String val=(text.substring(text.indexOf("l")+1,text.length())); 
              drive.left.maximum(val.toInt());
              saveConfig();
              Sprintln("Left Threshold: " + val);
            }

            if(text.startsWith("a")){
              String val=(text.substring(text.indexOf("a")+1,text.length())); 
              sensors.left.threshold = val.toInt();
              saveConfig();
              Sprintln("Left Threshold: " + val);
            }
            if(text.startsWith("b")){
              String val=(text.substring(text.indexOf("b")+1,text.length())); 
              sensors.right.threshold = val.toInt();
              saveConfig();
              Sprintln("Right Threshold: " + val);
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



