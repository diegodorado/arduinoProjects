#include <MIDI.h>
#include "YM2612.h"

MIDI_CREATE_DEFAULT_INSTANCE();
YM2612 ym;

int notes[] = {617,   653,   692,   733,   777,   823,   872,   924,   979,  1037,  1099,  1164};
int notes_down = 0;
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  notes_down++;
  ym.setFrequency(notes[pitch%12], pitch/12);
  ym.keyOn();
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  notes_down--;
  if(notes_down==0)
    ym.keyOff();
}


void handleControlChange(byte channel, byte number, byte value)
{
  switch(number) {

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
      ym.setOperator(0,value);
      break;
   case 52:
      ym.setOperator(1,value);
      break;
   case 53:
      ym.setOperator(2,value);
      break;
   case 54:
      ym.setOperator(3,value);
      break;
   case 55:
      ym.setAlgorithm(value);
      break;  
   case 56:
      ym.setChannel(value);
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

void loop() {
  MIDI.read();
}
