#ifndef WebSocketServer_h
#define WebSocketServer_h

#include <WebSocketsServer.h>

#define PROXIMITY_INTERVAL 1000
WebSocketsServer webSocket = WebSocketsServer(81);




 // state machine states
unsigned int state;
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
          state = GET_SAMPLE;
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
              
            Serial.print ( "Set Steering Motor Power (L,R):");
            Serial.print (motorleft);
            Serial.print (", ");
            Serial.println (motorright);
          }


           if(text.startsWith("p")){
            String yVal=(text.substring(text.indexOf("p")+1,text.length())); 
            power = yVal.toInt();
            motorright= power*rightmotor>>8;
            motorleft= power*leftmotor>>8;      
            analogWrite(RIGHT,motorright);
            analogWrite(LEFT,motorleft);
            
            Serial.print ( "Set Motor Power (L,R):");
            Serial.print (motorleft);
            Serial.print (", ");
            Serial.println (motorright);
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
        }
        break;
        
        case WStype_BIN:
            hexdump(payload, length);
            // echo data back to browser
            webSocket.sendBIN(num, payload, length);
            break;
    }
}
  
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
  return;
  }
}

#endif
