#define SEQUENCE_IDLE 0x00
#define GET_SAMPLE 0x10
#define GET_SAMPLE__WAITING 0x12 



  //----------------------------------------------------------------------- 
void sendProximity() {
  if (sensors.left.done & sensors.right.done) {
    Sprint("Sending Proximity");
    sensors.left.done = false;
    sensors.right.done = false;
    if (drive.auto_en()){
    if (sensors.left.limit & sensors.right.limit) {
          drive.turn180();
    }
    else if(sensors.left.limit) {
      drive.turnRight();
    }
    else if(sensors.right.limit) {
      drive.turnLeft();
    }
    }
    webSocket.sendTXT(socketNumber, "{\"left\":" + String(sensors.left.value) + ",\"right\":" + String(sensors.right.value) + "}");
  }
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
          String cmdtype = text.substring(0,3);
          Serial.println(cmdtype);
          if(cmdtype=="GET"){
            String cmd = text.substring(4,7);
            Serial.println(cmd);
            if(cmd=="RED") {
              char toSend[100];
              red.configJSON(toSend, sizeof(toSend));
              webSocket.sendTXT(num, toSend);
              Sprintln("Sending red LED Settings");
            }
            if(cmd=="BLU") {
              char toSend[100];
              blue.configJSON(toSend, sizeof(toSend));
              webSocket.sendTXT(num, toSend);
              Sprintln("Sending blue LED Settings");
             }
             if(cmd=="IRF") {
              char toSend[100];
              ir.configJSON(toSend, sizeof(toSend));
              webSocket.sendTXT(num, toSend);
              Sprintln("Sending front IR LED Settings");
             } 
             if(cmd=="IRB") {
              char toSend[100];
              irRear.configJSON(toSend, sizeof(toSend));
              webSocket.sendTXT(num, toSend);
              Sprintln("Sending rear IR LED Settings");
             }        
          }
          else if (cmdtype=="SET") {
            String cmd = text.substring(4,7);
            String action = text.substring(8,11);
            if(cmd=="RED") {
              if (action=="BLI") red.blinkOn();
              else if (action=="ENA") red.on();
              else if (action=="DIS") red.off();
              else if (action=="CAL") red.updateConfig((char *) &payload[12]);
            }
            if(cmd=="BLU") {
              if (action=="BLI") blue.blinkOn();
              else if (action=="ENA") blue.on();
              else if (action=="DIS") blue.off();
              else if (action=="CAL") blue.updateConfig((char *) &payload[12]);
            }
            if(cmd=="IRF") {
              if (action=="BLI") ir.blinkOn();
              else if (action=="ENA") ir.on();
              else if (action=="DIS") ir.off();
              else if (action=="CAL") ir.updateConfig((char *) &payload[12]);
            }
            if(cmd=="IRR") {
              if (action=="BLI") irRear.blinkOn();
              else if (action=="ENA") irRear.on();
              else if (action=="DIS") irRear.off();
              else if (action=="CAL") irRear.updateConfig((char *) &payload[12]);
            }
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

           if(text=="PROX_EN") {
            sensors.enable= true;
            Sprintln("Run Proximity Sensor");
           }

           if(text=="PROX_DIS") {
            sensors.enable = false;
            Sprintln("Stop Proximity Sensor");
           }

           if(text=="AUTO_EN") {
            sensors.enable= true;
            drive.startAutonomous();
            Sprintln("Starting Autonomous operation");
           }

           if(text=="AUTO_DIS") {
            sensors.enable= false;
            drive.stopAutonomous();
            Sprintln("Stopping Autonomous operation");
           }
           
           if(text=="START"){
             drive.enable();
             drive.set();
            }

           if(text=="STOP"){
             drive.stop();
             drive.disable();
            }
            
            if(text.startsWith("s")) {
              String xVal=(text.substring(text.indexOf("s")+1,text.length())); 
              drive.enable();
              drive.steer(xVal.toInt());
            }


           if(text.startsWith("p")){
            String yVal=(text.substring(text.indexOf("p")+1,text.length()));
            drive.enable();
            drive.power(yVal.toInt());
           }

           if(text.startsWith("q")){
            String yVal=(text.substring(text.indexOf("q")+1,text.length())); 
            drive.power(yVal.toInt());
           }

           if(text.startsWith("e")){
            String yVal=(text.substring(text.indexOf("e")+1,text.length()));
            drive.left.value(yVal.toInt());
            drive.left.set();
           }

            if(text.startsWith("f")){
            String yVal=(text.substring(text.indexOf("f")+1,text.length()));
            drive.right.value(yVal.toInt());
            drive.right.set();
           }

            if(text.startsWith("r")){
              String val=(text.substring(text.indexOf("r")+1,text.length())); 
              drive.right.maximum(val.toInt());
              Sprintln("Right Threshold: " + val);
            }    

            if(text.startsWith("l")){
              String val=(text.substring(text.indexOf("l")+1,text.length())); 
              drive.left.maximum(val.toInt());
              Sprintln("Left Threshold: " + val);
            }

            if(text.startsWith("a")){
              String val=(text.substring(text.indexOf("a")+1,text.length())); 
              sensors.left.threshold = val.toInt();
              Sprintln("Left Threshold: " + val);
            }
            if(text.startsWith("b")){
              String val=(text.substring(text.indexOf("b")+1,text.length())); 
              sensors.right.threshold = val.toInt();
              Sprintln("Right Threshold: " + val);
            }
            if(text.startsWith("c")){
              String val=(text.substring(text.indexOf("c")+1,text.length())); 
              sensors.cycles = val.toInt();
              Sprintln("Cycles: " + val);
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



