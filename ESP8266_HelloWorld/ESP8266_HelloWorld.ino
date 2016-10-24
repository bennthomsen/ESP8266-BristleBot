/* Hello World for ESP8266
 *  This program extends the Blink program on writes "Hello World" to the serial port.
 *  Once the code is uploaded to the ESP8266 you can monitor the output sing the\
 *  Arduino Serial Port Monitor which is opened by clicking the Magnifying class icon on
 *  the top righthand corner of this window. NOTE: The serial monitor Baud must be set to 115200
 *  using the the dropdown menu in the bottom righthand corner of the Serial Monitor window
 *  
 *  Benn Thomsen, Oct 2016
 */

// Define pin assignments
#define REDLED 16              // Corresponds to GPIO16 labelled pin D0 on NodeMCU board

int count = 0;                // define a count variable

// The setup function runs code that is only executed once at the begining of the program
void setup() {
  Serial.begin(115200);        // Initialize the serial port with a Baud rate of 115200bps
  pinMode(REDLED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
} // End of setup

// The code insiude the loop function is repeatedly executed forever
void loop() {
  Serial.println("Hello World");  // Write Hello World to the serial port
  
  count = count + 1;              // increment the count value (Note: the increment operation is often written as: count++)
  //count++;
  Serial.print("Current count: ");
  Serial.println(count);

  // Blink LED
  digitalWrite(REDLED, LOW);      // Turn the LED on
  delay(50);                      // Wait for 50ms
  digitalWrite(REDLED, HIGH);     // Turn the LED off by making the voltage HIGH
  delay(3000);                    // Wait for 3s
} // End of loop
