#ifndef WebSocketServer_h
#define WebSocketServer_h

#include <WebSocketsServer.h>

WebSocketsServer webSocket = WebSocketsServer(81);

 // state machine states
unsigned int state;
uint8_t socketNumber;
#define SEQUENCE_IDLE 0x00
#define GET_SAMPLE 0x10
#define GET_SAMPLE__WAITING 0x12 

  //----------------------------------------------------------------------- 

  void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
          break;
        case WStype_CONNECTED: {
          IPAddress ip = webSocket.remoteIP(num);
          socketNumber = num; 

         }
          break;
        case WStype_TEXT:{
            String text = String((char *) &payload[0]);
            
          if(text=="LED") {
            digitalWrite(REDLED,LOW);
            delay(500);
            digitalWrite(REDLED,HIGH);
            Sprintln("led just lit");
            String reply = "{\"led\":\"ON\"}";
            webSocket.sendTXT(num, reply);
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
           
          if(text.startsWith("s")) {
              String xVal=(text.substring(text.indexOf("s")+1,text.length())); 
              steer = xVal.toInt();
              if (steer < 0) {
                motorleft = (256+steer)*power*leftmotor>>16;
                analogWrite(LEFT,motorleft);
                motorright= power*rightmotor>>8;
                analogWrite(RIGHT,motorright);
              }
              else {
                motorright = (256-steer)*power*rightmotor>>16;
                analogWrite(LEFT,motorleft);
                motorleft= power*leftmotor>>8;
                analogWrite(RIGHT,motorright);
              }
              
            Sprint( "Set Motor Steering & Power (L,R):");
            Sprint(motorleft);
            Sprint(", ");
            Sprintln(motorright);
          }


           if(text.startsWith("p")){
            String yVal=(text.substring(text.indexOf("p")+1,text.length())); 
            power = yVal.toInt();
            motorright= power*rightmotor>>8;
            motorleft= power*leftmotor>>8;      
            analogWrite(RIGHT,motorright);
            analogWrite(LEFT,motorleft);
            
            Sprint( "Set Motor Power (L,R):");
            Sprint(motorleft);
            Sprint(", ");
            Sprintln(motorright);
           }

           if(text.startsWith("e")){
            String yVal=(text.substring(text.indexOf("e")+1,text.length())); 
            motorleft = yVal.toInt();     
            analogWrite(LEFT,motorleft);
            
            Sprint( "Set Motor Power (L):");
            Sprintln(motorleft);
           }

            if(text.startsWith("f")){
            String yVal=(text.substring(text.indexOf("f")+1,text.length())); 
            motorright = yVal.toInt();     
            analogWrite(RIGHT,motorright);
            
            Sprint( "Set Motor Power (R):");
            Sprintln(motorright);
           }

           if(text=="RESET"){
             analogWrite(RIGHT,LOW);
             analogWrite(LEFT,LOW);
             power = 0;
             Sprintln("reset");
            }

            if(text.startsWith("r")){
              String val=(text.substring(text.indexOf("r")+1,text.length())); 
              rightmotor = val.toInt();
              Sprintln("Right Threshold: " + val);
            }    

            if(text.startsWith("l")){
              String val=(text.substring(text.indexOf("l")+1,text.length())); 
              leftmotor = val.toInt();
              Sprintln("Left Threshold: " + val);
            }

            if(text.startsWith("a")){
              String val=(text.substring(text.indexOf("a")+1,text.length())); 
              leftProxSlope = val.toInt();
              Sprintln("Left Proximity Slope: " + val);
            }    

            if(text.startsWith("b")){
              String val=(text.substring(text.indexOf("b")+1,text.length())); 
              leftProxOffset = val.toInt();
              Sprintln("Left Proximity Offset: " + val);
            }

            if(text.startsWith("c")){
              String val=(text.substring(text.indexOf("c")+1,text.length())); 
              rightProxSlope = val.toInt();
              Sprintln("Right Proximity Slope: " + val);
            }    

            if(text.startsWith("d")){
              String val=(text.substring(text.indexOf("d")+1,text.length())); 
              rightProxOffset = val.toInt();
              Sprintln("Right Proximity Offset: " + val);
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

void sendProximity() {
          if (LeftAvailable) {
            LeftAvailable = false;
            float LeftProx = ProximityCal(pulselengthL, leftProxSlope, leftProxOffset);
            Sprintln(LeftProx);
            webSocket.sendTXT(socketNumber, "{\"left\":" + String(pulselengthL) + ",\"leftCal\":" + String(LeftProx) + "}");
          }
          if (RightAvailable) {
            RightAvailable = false;
            float RightProx = ProximityCal(pulselengthL, rightProxSlope, rightProxOffset);
            Sprintln(RightProx);
            webSocket.sendTXT(socketNumber, "{\"right\":" + String(pulselengthR) + ",\"rightCal\":" + String(RightProx) + "}");
          }
}

#endif
