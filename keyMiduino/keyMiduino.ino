// Analog in 0: Grain 1 pitch 
// Analog in 1: Grain 2 decay 
// Analog in 2: Grain 1 decay 
// Analog in 3: Grain 2 pitch 
// Digital 3: Audio out (Digital 11 on ATmega8) 

#include <avr/io.h> 
#include <avr/interrupt.h> 
//#include <MIDI.h>
#include "LinkedList.h"
#include <PS2KeyAdvanced.h>

#define KB_DATA_PIN 4
#define KB_IRQ_PIN  2

PS2KeyAdvanced keyboard;

//struct CustomMidiSettings : public midi::DefaultSettings {
//  static const unsigned BaudRate = 9600;
//};
//MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial,Serial,MIDI,CustomMidiSettings);
 
uint16_t syncPhaseAcc; 
uint16_t syncPhaseInc; 
uint16_t grainPhaseAcc; 
uint16_t grainPhaseInc; 
uint16_t grainAmp; 
uint8_t grainDecay; 
uint16_t grain2PhaseAcc; 
uint16_t grain2PhaseInc; 
uint16_t grain2Amp; 
uint8_t grain2Decay; 

uint8_t grainPhaseIncMidiCC; 
uint8_t grainDecayMidiCC; 
uint8_t grain2PhaseIncMidiCC; 
uint8_t grain2DecayMidiCC; 

LinkedList<uint8_t> notes_stack = LinkedList<uint8_t>();
      

#define GRAIN_FREQ_CONTROL   (1) 
#define GRAIN_DECAY_CONTROL  (2) 
#define GRAIN2_FREQ_CONTROL  (3) 
#define GRAIN2_DECAY_CONTROL (4) 


#define PWM_PIN       3 
#define PWM_VALUE     OCR2B 
#define PWM_INTERRUPT TIMER2_OVF_vect 

// Smooth logarithmic mapping 
// 
uint16_t antilogTable[] = { 
64830,64132,63441,62757,62081,61413,60751,60097,59449,58809,58176,57549,56929,56316,55709,55109,
54515,53928,53347,52773,52204,51642,51085,50535,49991,49452,48920,48393,47871,47356,46846,46341,
45842,45348,44859,44376,43898,43425,42958,42495,42037,41584,41136,40693,40255,39821,39392,38968,
38548,38133,37722,37316,36914,36516,36123,35734,35349,34968,34591,34219,33850,33486,33125,32768 
}; 
uint16_t mapPhaseInc(uint16_t input) { 
  return (antilogTable[input & 0x3f]) >> (input >> 6); 
} 

uint16_t midiTable[] = { 
17,18,19,20,22,23,24,26,27,29,31,32,34,36,38,41,43,46,48,51,54,58,61,65,69,73,
77,82,86,92,97,103,109,115,122,129,137,145,154,163,173,183,194,206,218,231,
244,259,274,291,308,326,346,366,388,411,435,461,489,518,549,581,616,652,691,
732,776,822,871,923,978,1036,1097,1163,1232,1305,1383,1465,1552,1644,1742,
1845,1955,2071,2195,2325,2463,2610,2765,2930,3104,3288,3484,3691,3910,4143,
4389,4650,4927,5220,5530,5859,6207,6577,6968,7382,7821,8286,8779,9301,9854,
10440,11060,11718,12415,13153,13935,14764,15642,16572,17557,18601,19708,20879,
22121,23436,24830,26306 
}; 
uint16_t mapMidi(uint16_t input) { 
  return (midiTable[(1023-input) >> 3]); 
} 


uint8_t kb2Midi[] = { 
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,63,0,49,51,0,54,56,58,0,61,0,48,0,50,52,0,0,0,43,40,39,52,0,42,44,
60,46,0,49,47,45,62,64,48,53,37,55,59,41,50,38,57,36,51,0,65,67,66,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
}; 

void audioOn() { 
  // Set up PWM to 31.25kHz,phase accurate 
  TCCR2A = _BV(COM2B1) | _BV(WGM20); 
  TCCR2B = _BV(CS20); 
  TIMSK2 = _BV(TOIE2); 
} 



void readKb(){
  if( keyboard.available() ) {
    uint16_t c = keyboard.read();
    if( c > 0 ) {
          //Serial.println( c,HEX );
      uint8_t k = c & 0xFF;
      uint8_t m = kb2Midi[k];
      uint8_t s = c >> 8;
      if(m>0){
        if( s==0){
          handleNoteOn(0,m,100);
        }else{
          handleNoteOff(0,m,100);
        }
      }else{
        //not a note

        if( s==1){
          switch(k) {
             case 0x61:
                if(grainPhaseIncMidiCC>0) grainPhaseIncMidiCC--;
                break;
             case 0x62:
                if(grainPhaseIncMidiCC<127) grainPhaseIncMidiCC++;
                break;
             case 0x63:
                if(grainDecayMidiCC>0) grainDecayMidiCC--;
                break;
             case 0x64:
                if(grainDecayMidiCC<127) grainDecayMidiCC++;
                break;
             case 0x65:
                if(grain2PhaseIncMidiCC>0) grain2PhaseIncMidiCC--;
                break;
             case 0x66:
                if(grain2PhaseIncMidiCC<127) grain2PhaseIncMidiCC++;
                break;
             case 0x67:
                if(grain2DecayMidiCC>0) grain2DecayMidiCC--;
                break;
             case 0x68:
                if(grain2DecayMidiCC<127) grain2DecayMidiCC++;
                break;
          }          
                    Serial.println( k,DEC );
                    Serial.println( grainPhaseIncMidiCC,DEC );

          grainPhaseInc  = mapPhaseInc(grainPhaseIncMidiCC*8) / 2; 
          grainDecay     = grainDecayMidiCC*2; 
          grain2PhaseInc = mapPhaseInc(grain2PhaseIncMidiCC*8) / 2; 
          grain2Decay    = grainDecayMidiCC*2; 
          
        }

        
        
        
      }
    }
  }
}


void handleNoteOn(byte channel,byte pitch,byte velocity) {
  // remove note from the stack if already there
  for(int n = 0;n<notes_stack.size();n++){
    if( pitch == notes_stack.get(n)){
      notes_stack.remove(n);
    }        
  }
  
  notes_stack.add(pitch);
  syncPhaseInc = midiTable[pitch]; 
}

void handleNoteOff(byte channel,byte pitch,byte velocity) {
  if(notes_stack.size()>0){
    // if pitch corresponds to last note in the stack
    // set syncPhaseInc to previous note (if exists)
    if( pitch == notes_stack.get(notes_stack.size()-1)){
      notes_stack.pop();
      if(notes_stack.size()>0){
        uint8_t previous = notes_stack.get(notes_stack.size()-1);
        syncPhaseInc = midiTable[previous]; 
      }           
    }
  }

  // remove notes from the stack
  for(int n = 0;n<notes_stack.size();n++){
    if( pitch == notes_stack.get(n)){
      notes_stack.remove(n);
    }        
  }

  // stack empty? trigger keyoff          
  if(notes_stack.size()==0){
    syncPhaseInc = 0;
  }  
}

void setup() { 
  pinMode(PWM_PIN,OUTPUT); 
  audioOn(); 
  
  //MIDI.setHandleNoteOn(handleNoteOn);
  //MIDI.setHandleNoteOff(handleNoteOff);
  //MIDI.begin(MIDI_CHANNEL_OMNI);


  keyboard.begin( KB_DATA_PIN,KB_IRQ_PIN );
  Serial.begin( 9600 );
  Serial.println( "PS2 Advanced Key Simple Test:" );


  grainPhaseInc  = 10000;//mapPhaseInc(analogRead(GRAIN_FREQ_CONTROL)) / 2; 
  grainDecay     = 60;//analogRead(GRAIN_DECAY_CONTROL) / 8; 
  grain2PhaseInc = 10000;//mapPhaseInc(analogRead(GRAIN2_FREQ_CONTROL)) / 2; 
  grain2Decay    = 60;//analogRead(GRAIN2_DECAY_CONTROL) / 4; 

} 

void loop() { 
  //MIDI.read();


  readKb();
  
} 

SIGNAL(PWM_INTERRUPT) 
{ 
  uint8_t value; 
  uint16_t output; 

  syncPhaseAcc += syncPhaseInc; 
  if (syncPhaseAcc < syncPhaseInc) { 
    // Time to start the next grain 
    grainPhaseAcc = 0; 
    grainAmp = 0x7fff; 
    grain2PhaseAcc = 0; 
    grain2Amp = 0x7fff; 
  } 

  // Increment the phase of the grain oscillators 
  grainPhaseAcc += grainPhaseInc; 
  grain2PhaseAcc += grain2PhaseInc; 

  // Convert phase into a triangle wave 
  value = (grainPhaseAcc >> 7) & 0xff; 
  if (grainPhaseAcc & 0x8000) value = ~value; 
  // Multiply by current grain amplitude to get sample 
  output = value * (grainAmp >> 8); 

  // Repeat for second grain 
  value = (grain2PhaseAcc >> 7) & 0xff; 
  if (grain2PhaseAcc & 0x8000) value = ~value; 
  output += value * (grain2Amp >> 8); 

  // Make the grain amplitudes decay by a factor every sample  (exponential decay) 
  grainAmp -= (grainAmp >> 8) * grainDecay; 
  grain2Amp -= (grain2Amp >> 8) * grain2Decay; 

  // Scale output to the available range,clipping if necessary 
  output >>= 9; 
  if (output > 255) output = 255; 

  // Output to PWM (this is faster than using analogWrite) 
  PWM_VALUE = output; 
}
