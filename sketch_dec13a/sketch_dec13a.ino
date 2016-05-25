//Ramp out
//by Amanda Ghassaei
//http://www.instructables.com/id/Arduino-Audio-Output/
//Sept 2012

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

void setup(){
  //set digital pins 0-7 as outputs
  for (int i=0;i<8;i++){
    pinMode(i,OUTPUT);
  }
}

void loop(){
  for (int a=0;a<256;a++){
    PORTD = a;//send out  to digital pins 0-7
    delay(6);
  }
  for (int a=255;a>0;a--){
    PORTD = a;//send out  to digital pins 0-7
    delay(2);
  }
  
}

