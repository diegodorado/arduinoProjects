#include "AutoRange.h"

int jumperPins[]= {8,9,10,11};
int lastValues[4];

AutoRange ar0 = AutoRange(A0,// analog pin
                          200, // interpolation time in ms
                          0, // min output value
                          127);  // max output value 
AutoRange ar1 = AutoRange(A1,// analog pin
                          200, // interpolation time in ms
                          0, // min output value
                          127);  // max output value 
AutoRange ar2 = AutoRange(A2,// analog pin
                          200, // interpolation time in ms
                          0, // min output value
                          127);  // max output value 
AutoRange ar3 = AutoRange(A3,// analog pin
                          200, // interpolation time in ms
                          0, // min output value
                          127);  // max output value 


AutoRange ars[4] = {ar0,ar1,ar2,ar3};
int arPins[] = {A0,A1,A2,A3};
int ccs[] = {70,71,72,73};

void setup() {
  for(int i=0;i<4;i++){
    pinMode(jumperPins[i],INPUT_PULLUP);  
  }
  
  Serial.begin(9600);

}

void loop() {
  
  int jumper[4];
 
  boolean mapping = false;
  for(int i=0;i<4;i++){
    jumper[i] = digitalRead(jumperPins[i]);  
    if(jumper[i]==0)
      mapping =true;
  }

  for(int i=0;i<4;i++){
    int value= ars[i].read();
    if(mapping){
      if(jumper[i]==0){
        sendCc(ccs[i],value);
        delay(1000);
      }
    }else{
      if(lastValues[i] != value){
        sendCc(ccs[i],value);
        lastValues[i] = value;
      }
    }
  }

  delay(10);

}


// sends a Control Change
void sendCc(byte cc, byte val) {
  //return;
  Serial.write(0xB0);
  Serial.write(cc);
  Serial.write(val);
}

