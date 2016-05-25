/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13.
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead().

 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground

 * Note: because most Arduinos have a built-in LED attached
 to pin 13 on the board, the LED is optional.


 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/AnalogInput

 */

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int threshold = 200;

int noteON = 144;//144 = 10010000 in binary, note on command
 int noteOFF = 128;//128 = 10000000 in binary, note off command
bool triggered=false;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
    //Serial.begin(38400);

}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
    
  if(sensorValue>threshold && !triggered){
    MIDImessage(noteON, 60, sensorValue/8);//turn note on
    delay(1000);
    triggered=true;
  }else{
    triggered=false;
  }
}
