/*
BristleBot motor speed control
*/

//pin assignments
#define REDLED 16

#define RIGHTMOTOR 15
#define LEFTMOTOR 5
#define DELAY 100


void setup() {                
  // initialize the digital pin as an output.
  pinMode(REDLED, OUTPUT);   
  pinMode(RIGHTMOTOR, OUTPUT); 
  pinMode(LEFTMOTOR, OUTPUT); 
  
  analogWriteFreq(400);    // Set the pulse frequency of the PWM motor drive signal
  
  analogWrite(RIGHTMOTOR,0);  //Set the Right motor power (0-255)
  analogWrite(LEFTMOTOR,0);   //Set the Left motor power (0-255)
}
   
// the loop routine runs over and over again forever:
void loop() {
  int level = 0;\
  digitalWrite(REDLED, LOW);   // turn the LED on (HIGH is the voltage level)
  while (level < 255) {
    analogWrite(RIGHTMOTOR,level);  //Set the Right motor power (0-255)
    analogWrite(LEFTMOTOR,level);   //Set the Left motor power (0-255)
    level = level + 10;
    delay(DELAY);
  }
  digitalWrite(REDLED, HIGH);    // turn the LED off by making the voltage LOW
  while (level > 0) {
    analogWrite(RIGHTMOTOR,level);  //Set the Right motor power (0-255)
    analogWrite(LEFTMOTOR,level);   //Set the Left motor power (0-255)
    level = level - 10;
    delay(DELAY);
  }
}

