/* Blink programmes the NodeMCU board to turn on and off the LED that is near the USB port on the NodeMCU.
 *  blink is used to test the board and your programming environment in the same way that "hello world" is
 *  to test a computer program
 */

// Define pin assignments
#define REDLED 16              // Corresponds to GPIO16 labelled pin D0 on NodeMCU board

// The setup function runs code that is only executed once at the begining of the program
void setup() {
  pinMode(REDLED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

// The code insiude the loop function is repeatedly executed forever
void loop() {
  digitalWrite(REDLED, LOW);   // Turn the LED on (Note that LOW is the voltage level
                               // but actually the LED is on, this is because
                               // it is active low on the ESP-01)
  delay(1000);                 // Wait for a second
  digitalWrite(REDLED, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                 // Wait for two seconds (to demonstrate the active low LED)
}
