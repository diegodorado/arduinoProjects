#include <MIDI.h>
#include "YM2612.h"

MIDI_CREATE_DEFAULT_INSTANCE();
YM2612 ym;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  ym.noteOn(channel, pitch, velocity);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  ym.noteOff(channel, pitch, velocity);
}


void handleControlChange(byte channel, byte number, byte value)
{
  switch(number) {

   case 1:
      ym.setLFO(value);
      break;
   case 21:
      //ym.setAttackRate(value);
      break;      
   case 22:
      //ym.setAttackRate(value);
      break;      
   case 23:
      //ym.setAttackRate(value);
      break;      
   case 24:
      //ym.setAttackRate(value);
      break;      
   case 25:
      //ym.setAttackRate(value);
      break;      
   case 26:
      ym.setStereo(value);
      break;      
   case 27:
      ym.setAMS(value);
      break;      
   case 28:
      ym.setFMS(value);
      break;      
     
   case 41:
      ym.setAttackRate(value);
      break;      
   case 42:
      ym.setDecayRate(value);
      break;
   case 43:
      ym.setSustainRate(value);
      break;
   case 44:
      ym.setReleaseRate(value);
      break;
   case 45:
      ym.setTotalLevel(value);
      break;
   case 46:
      ym.setSustainLevel(value);
      break;
   case 47:
      ym.setMultiply(value);
      break;
   case 48:
      ym.setDetune(value);
      break;
   case 49:
      ym.setRateScaling(value);
      break;
   case 51:
      ym.selectOperator(0,value);
      break;
   case 52:
      ym.selectOperator(1,value);
      break;
   case 53:
      ym.selectOperator(2,value);
      break;
   case 54:
      ym.selectOperator(3,value);
      break;
   case 55:
      ym.setAlgorithm(value);
      break;  
   case 56:
      ym.selectChannel(value);
      break;  
   case 57:
      ym.setUnison(value);
      break;  

      
  }
  
}



void setup() {
  ym.setup(A5,A4,A3,A2,A1,A0,9, 10,11,2,3,4,5,6,7);
  ym.initialize();

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
}

int note = 0;
uint32_t previousMillis = 0;
int interval = 700;
bool pressed = false;

void loop() {
  MIDI.read();
  ym.update();

  uint32_t currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if(pressed){
      ym.noteOff(1, 60+note, 0);
      note++;
      note %=12;
      pressed = false;
    }else{
      ym.noteOn(1, 60+note, 100);
      pressed = true;
    }

  }
  
}
