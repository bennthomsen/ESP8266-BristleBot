/*
BristleBot motor speed control
*/

//pin assignments
#define REDLED 16

#define RIGHTMOTOR 15
#define LEFTMOTOR 5


void setup() {                
  // initialize the digital pin as an output.
  pinMode(REDLED, OUTPUT);   
  pinMode(RIGHTMOTOR, OUTPUT); 
  pinMode(LEFTMOTOR, OUTPUT); 
  
  analogWriteFreq(400);    // Set the pulse frequency of the PWM motor drive signal
  
  analogWrite(RIGHTMOTOR,100);  //Set the Right motor power (0-255)
  analogWrite(LEFTMOTOR,100);   //Set the Left motor power (0-255)
}
   
// the loop routine runs over and over again forever:
void loop() {
  
  digitalWrite(REDLED, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(REDLED, HIGH);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}

