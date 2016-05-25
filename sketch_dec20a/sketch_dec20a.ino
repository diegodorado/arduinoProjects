void setup() {
  //analogWriteResolution(12);
}

void loop() {
  for(int i = 0;i<4096;i++){
    //analogWrite(DAC0, i);
    delay(10);
  }
}
