#include <MIDI.h>
#include "YM2612.h"

//MIDI_CREATE_DEFAULT_INSTANCE();
YM2612 ym;

int notes[] = {617,   653,   692,   733,   777,   823,   872,   924,   979,  1037,  1099,  1164};

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  ym.setFrequency(notes[pitch%12], pitch/12);
  ym.keyOn();
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  ym.keyOff();
}

/*

void handleControlChange(byte channel, byte number, byte value)
{
  switch(number) {

   case 41:
      setAttackRate(value);
      break;
   case 42:
      setDecayRate(value);
      break;
   case 43:
      setSustainRate(value);
      break;
   case 44:
      setReleaseRate(value);
      break;
   case 45:
      setTotalLevel(value);
      break;
   case 46:
      setSustainLevel(value);
      break;
   case 47:
      setMultiply(value);
      break;
   case 48:
      setDetune(value);
      break;
   case 49:
      setRateScaling(value);
      break;
   case 51:
      operatorSelection(1,value);
      break;
   case 52:
      operatorSelection(2,value);
      break;
   case 53:
      operatorSelection(3,value);
      break;
   case 54:
      operatorSelection(4,value);
      break;
   case 55:
      setAlgorithm(value);
      break;  
  }
  
}
*/

void setup() {

  ym.setup(A5,A4,A3,A2,A1,A0,9, 10,11,2,3,4,5,6,7);
  ym.initialize();

  //MIDI.setHandleNoteOn(handleNoteOn);
  //MIDI.setHandleNoteOff(handleNoteOff);
  //MIDI.setHandleControlChange(handleControlChange);
  //MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  //MIDI.read();
  ym.keyOn();
  delay(1000);
  ym.keyOff();
  delay(1000);
}
