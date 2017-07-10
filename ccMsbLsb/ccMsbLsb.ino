

const int numReadings = 250;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int inputPin = A0;

void setup() {     
 Serial.begin(9600);    

 // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  
}

void loop() {


  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;

  unsigned int a = average;
  
  byte msb = (a >> 7) & B01111111;
  byte lsb = a & B01111111;

  //SendMidiCC( 2, msb);
  SendMidiCC( 25, lsb);

  
  
  delay(1);        // delay in between reads for stability

}

void SendMidiCC(byte byte2, byte byte3){
  Serial.write(0xB0);
  Serial.write(byte2);
  Serial.write(byte3);
}

