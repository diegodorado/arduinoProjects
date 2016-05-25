/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

// the setup routine runs once when you press reset:
void setup() {     
 Serial.begin(9600);  
  // initialize the digital pin as an output.
  
}

// the loop routine runs over and over again forever:
void loop() {
  
  int a0 = analogRead(0);
  
  Serial.println(a0);
  delay(250);        // delay in between reads for stability
  


}
