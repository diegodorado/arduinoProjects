#include <MIDI.h>
#include "YM2612.h"

MIDI_CREATE_DEFAULT_INSTANCE();

YM2612 ym;
bool play_seq = false;
uint32_t interval = 300;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  ym.noteOn(channel, pitch, velocity);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  ym.noteOff(channel, pitch, velocity);
}


void handlePitchBend(byte channel, int bend)
{
  ym.pitchBend(channel, bend);
}


void handleControlChange(byte channel, byte number, byte value)
{

  UNUSED(channel);
  
  switch(number) {

   // mod wheel
   case 1:
      ym.setLFO(value);
      break;

   // encoders
   case 21:
      ym.setPlaymode(value);
      break;      
   case 22:
      //ym.setAttackRate(value);
      break;      
   case 23:
      //ym.setAttackRate(value);
      break;      
   case 24:
      ym.setFeedback(value);
      //interval = 10 * value;
      break;      
   case 25:
      ym.selectPatch(value);
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

   // faders
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

   // toggles
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
   case 58:
      ym.setAmplitudeModulation(value);
      break;  
      
  // transport
   case 112:
      break;
   case 113:
      break;
   case 114:
      play_seq = false;
      break;
   case 115:
      play_seq = true;
      break;
   case 116:
      ym.loadPatch();
      break;
   case 117:
      break;
      
  }
  
}



void setup() {
  ym.setup(A5,A4,A3,A2,A1,A0,9, 6,7,5,4);
  ym.initialize();

  digitalWrite(0, HIGH);  // pullup for normal logic!
  
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandlePitchBend(handlePitchBend);

  
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
}



void loop() {
  MIDI.read();
  ym.update();

  if(play_seq)
    playSeq();
    
}
void playSeq(){
  static uint8_t note = 0;
  static uint32_t previousMillis = 0;
  
  static bool pressed = false;  
  uint32_t currentMillis = millis();
  uint8_t basenote = 100;
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if(pressed){
      ym.noteOff(1, basenote+note, 0);
      note++;
      note %=12;
      pressed = false;
    }else{
      ym.noteOn(1, basenote+note, 100);
      pressed = true;
    }

  }  
}
