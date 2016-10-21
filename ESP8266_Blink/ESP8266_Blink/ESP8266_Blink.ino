#define REDLED 16   // Corresponds to GPIO16 labelled pin D0 on NodeMCU board

void setup() {
  pinMode(REDLED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(REDLED, LOW);   // Turn the LED on (Note that LOW is the voltage level
                               // but actually the LED is on; this is because 
                               // it is acive low on the ESP-01)
  delay(1000);                 // Wait for a second
  digitalWrite(REDLED, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                 // Wait for two seconds (to demonstrate the active low LED)
}
