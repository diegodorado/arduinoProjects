int inputPin = A0;
int breath = 0;
unsigned long millisPassed = 0;

void setup() {
  Serial.begin(9600);    

}

void loop() {
  unsigned long loopTime = millis() - millisPassed;
  millisPassed = millis();
    
  int b = analogRead(inputPin);
  
  b = map(b, 0, 1023, 0, 127);
  
  //breath = breath + (b-breath) * loopTime * 0.001 * 0.1;
  breath = breath + (b-breath) * 0.1;
  breath = constrain(breath, 0, 127);
  
  SendMidiCC( 25, breath);

  delay(1);        // delay in between reads for stability

}

void SendMidiCC(byte byte2, byte byte3){
  Serial.write(0xB0);
  Serial.write(byte2);
  Serial.write(byte3);
}
