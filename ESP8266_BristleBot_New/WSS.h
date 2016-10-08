#define SEQUENCE_IDLE 0x00
#define GET_SAMPLE 0x10
#define GET_SAMPLE__WAITING 0x12 



  //----------------------------------------------------------------------- 
void sendProximity() {
  if (sensors.left.done & sensors.right.done) {
    sensors.left.done = false;
    sensors.right.done = false;
    sensors.left.limit = (sensors.left.value > sensors.left.threshold);
    sensors.right.limit = (sensors.right.value > sensors.right.threshold);
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
    String toSend = "{\"left\":{\"value\":" + String(sensors.left.value) + ",\"limit\":" + String(sensors.left.limit) + "},\"right\":{\"value\":" + String(sensors.right.value) + ",\"limit\":" + String(sensors.right.limit) + "}}";
    webSocket.sendTXT(socketNumber,toSend);
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
          if(cmdtype=="GET"){
            String cmd = text.substring(4,7);
            Serial.println("GET:" + cmd);
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
             if(cmd=="BAT") {
              int adc = analogRead(A0);
              int voltage = (5222*adc) >> 10;
              String voltageData = "Supply Voltage: " + String(voltage) +"mV";
              Sprintln(voltageData);
              String reply = "{\"battery\":";
              reply += voltage;
              reply += "}";
              webSocket.sendTXT(num, reply);
             }
             if(cmd=="PRO") {
              char toSend[100];
              proxConfigJSON(toSend, sizeof(toSend));
              webSocket.sendTXT(num, toSend);
             }
             if(cmd=="DRI") {
              char toSend[100];
              drive.configJSON(toSend, sizeof(toSend));
              webSocket.sendTXT(num, toSend);
             }
                     
          }
          else if (cmdtype=="SET") {
            String cmd = text.substring(4,7);
            String action = text.substring(8,11);
            Serial.println("SET:" + cmd +":" +action);
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
            if(cmd=="DRI") {
              String val=(text.substring(11,text.length()));
              if (action=="LEF") {
                drive.left.value(val.toInt());
                drive.left.set();
              }
              else if (action=="RIG") {
                drive.right.value(val.toInt());
                drive.right.set();
              }
              else if (action=="LMA") drive.leftMax(val.toInt());
              else if (action=="RMA") drive.rightMax(val.toInt());
              else if (action=="AUT") {
                proximityEnable(val.toInt());
                drive.autonomousEnable(val.toInt());
              }
              else if (action=="STE") drive.steer(val.toInt());
              else if (action=="POW") drive.power(val.toInt());
              else if (action=="ENA") drive.enable(val.toInt());
            }
            if(cmd=="PRO") {
              String val=(text.substring(11,text.length()));
              if (action=="ENA") proximityEnable(val.toInt());
              else if (action=="PRO") proxUpdateConfig((char *) &payload[12]);
            }
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



