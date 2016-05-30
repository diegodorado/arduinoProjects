#include "YM2612.h"



void YM2612::setup(uint8_t ic_pin,
                   uint8_t cs_pin,
                   uint8_t wr_pin,
                   uint8_t rd_pin,
                   uint8_t a0_pin,
                   uint8_t a1_pin,
                   uint8_t mc_pin,
                   uint8_t data0_pin,
                   uint8_t data1_pin,
                   uint8_t data2_pin,
                   uint8_t data3_pin,
                   uint8_t data4_pin,
                   uint8_t data5_pin,
                   uint8_t data6_pin,
                   uint8_t data7_pin)
{
  this->ic_pin = ic_pin;
  this->cs_pin = cs_pin;
  this->wr_pin = wr_pin;
  this->rd_pin = rd_pin;
  this->a0_pin = a0_pin;
  this->a1_pin = a1_pin;
  this->mc_pin = mc_pin;
  this->data0_pin = data0_pin;
  this->data1_pin = data1_pin;
  this->data2_pin = data2_pin;
  this->data3_pin = data3_pin;
  this->data4_pin = data4_pin;
  this->data5_pin = data5_pin;
  this->data6_pin = data6_pin;
  this->data7_pin = data7_pin;
}


void YM2612::initialize() {
  /* Pins setup */
  pinMode(ic_pin, OUTPUT);
  pinMode(cs_pin, OUTPUT);
  pinMode(wr_pin, OUTPUT);
  pinMode(rd_pin, OUTPUT);
  pinMode(a0_pin, OUTPUT);
  pinMode(a1_pin, OUTPUT);
  pinMode(mc_pin, OUTPUT);

  pinMode(data0_pin, OUTPUT);
  pinMode(data1_pin, OUTPUT);
  pinMode(data2_pin, OUTPUT);
  pinMode(data3_pin, OUTPUT);
  pinMode(data4_pin, OUTPUT);
  pinMode(data5_pin, OUTPUT);
  pinMode(data6_pin, OUTPUT);
  pinMode(data7_pin, OUTPUT);

  /* IC, CS, WR and RD HIGH by default */
  digitalWrite(ic_pin, HIGH);
  digitalWrite(cs_pin, HIGH);
  digitalWrite(wr_pin, HIGH);
  digitalWrite(rd_pin, HIGH);

  /* A0 and A1 LOW by default */
  digitalWrite(a0_pin, LOW);
  digitalWrite(a1_pin, LOW);

  /* F_CPU / 2 clock generation */
  TCCR1A = _BV(COM1A0);            /* Toggle OCA1 on compare match */
  TCCR1B = _BV(WGM12) | _BV(CS10); /* CTC mode with prescaler /1 */
  TCCR1C = 0;                      /* Flag reset */
  TCNT1 = 0;                       /* Counter reset */
  OCR1A = 0;                       /* Divide base clock by two */

  /* Reset YM2612 */
  digitalWrite(ic_pin, LOW);
  delay(10);
  digitalWrite(ic_pin, HIGH);
  delay(10);


  setDefaults();

}




void YM2612::setDefaults() {

  unison = true;
  selected_channel = 1;
  operators[0] = true;
  operators[1] = true;
  operators[2] = true;
  operators[3] = true;

  for(int i=0;i<6;i++){
    voices[i].on = false;
    keyOff(i);
  }

  //channel params
  setFeedback(0);
  setAlgorithm(7);
  setStereo(3);
  setAMS(0);
  setFMS(7);
  //operator params
  setAmplitudeModulation(1);
  setAttackRate(31);
  setDecayRate(31);
  setSustainRate(0);
  setReleaseRate(8);
  setTotalLevel(0);
  setSustainLevel(0);
  setMultiply(2);
  setDetune(3);
  setPlaymode(POLY6x1);
  
}

void YM2612::setLFO (int value) { 
  //mod wheel value (0..8) is used both to enable
  // and to set lfo freq
  if(value==0){
    setMasterParameter( YM_MA_LFO_E, 0);    
  }else{
    setMasterParameter( YM_MA_LFO_E, 1);
    setMasterParameter( YM_MA_LFO_F, value-1);
  }
}

void YM2612::sendData(uint8_t data) {
  digitalWrite(cs_pin, LOW);

  digitalWrite(data0_pin, ( (data >> 0) & 0x01) );
  digitalWrite(data1_pin, ( (data >> 1) & 0x01) );
  digitalWrite(data2_pin, ( (data >> 2) & 0x01) );
  digitalWrite(data3_pin, ( (data >> 3) & 0x01) );
  digitalWrite(data4_pin, ( (data >> 4) & 0x01) );
  digitalWrite(data5_pin, ( (data >> 5) & 0x01) );
  digitalWrite(data6_pin, ( (data >> 6) & 0x01) );
  digitalWrite(data7_pin, ( (data >> 7) & 0x01) );

  delayMicroseconds(2); //it was 1 before
  digitalWrite(wr_pin, LOW);
  delayMicroseconds(5);
  digitalWrite(wr_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(cs_pin, HIGH);
}


void YM2612::setRegister(uint8_t part, uint8_t reg, uint8_t data) {
  digitalWrite(a1_pin, part);
  digitalWrite(a0_pin, LOW);
  sendData(reg);
  digitalWrite(a0_pin, HIGH);
  sendData(data);
}

void YM2612::setMasterParameter(int reg_offset, int val_size, int val_shift, int val) {
  uint8_t* p = (uint8_t *) ( &master) + reg_offset;

  *(p) &= ~(mask(val_size) << val_shift); //clean
  *(p) |= ((mask(val_size) & val) << val_shift); //write
  setRegister(0, YM_MASTER_ADDR + reg_offset, *(p));

}

void YM2612::setChannelParameter(int reg_offset, int val_size, int val_shift, int val) {

  if (unison) {
    for (int i = 0; i < 6; i++)
      setChannelParameter(i, reg_offset, val_size, val_shift, val);
  } else {
    setChannelParameter(selected_channel, reg_offset, val_size, val_shift, val);
  }

}

void YM2612::setChannelParameter(int chan, int reg_offset, int val_size, int val_shift, int val) {

  int channel_part = chan / 3;
  uint8_t* p = (uint8_t *) ( &channels[channel_part]) + reg_offset;
  int channel_offset = chan % 3; //which of the 12 registers

  *(p + channel_offset) &= ~(mask(val_size) << val_shift); //clean
  *(p + channel_offset) |= ((mask(val_size) & val) << val_shift); //write
  setRegister(channel_part, YM_CHN_ADDR + reg_offset + channel_offset, *(p + channel_offset));

}


void YM2612::setOperatorParameter(int reg_offset, int val_size, int val_shift, int val) {

  if (unison) {
    for (int i = 0; i < 6; i++)
      setOperatorParameter(i, reg_offset, val_size, val_shift, val);
  } else {
    setOperatorParameter(selected_channel, reg_offset, val_size, val_shift, val);
  }

}


void YM2612::setOperatorParameter(int chan, int reg_offset, int val_size, int val_shift, int val) {
  for (int i = 0; i < 4; i++)
    if (operators[i])
      setOperatorParameter(chan, i, reg_offset, val_size, val_shift, val);
}

void YM2612::setOperatorParameter(int chan,int oper, int reg_offset, int val_size, int val_shift, int val) {
  int channel_part = chan / 3;
  uint8_t* p = (uint8_t *) ( &channels[channel_part]) + reg_offset;
  int op_offset = chan % 3 + oper * 4; //which of the 12 registers
  *(p + op_offset) &= ~(mask(val_size) << val_shift); //clean
  *(p + op_offset) |= ((mask(val_size) & val) << val_shift); //write
  setRegister(channel_part, YM_CHN_ADDR + reg_offset + op_offset, *(p + op_offset));
}




void YM2612::setPlaymode(int value) {
  playmode = (playmode_t)value;

  // kill all voices
  for(int i = 0; i<6;i++){
    voices[i].on = false;
    keyOff(i);
  }
  notes_stack0.clear();
  notes_stack1.clear();
  
}

void YM2612::noteOn(byte channel, byte pitch, byte velocity)
{
  UNUSED(channel);
  UNUSED(velocity);


  switch(playmode) {

    case MONO1x6:
    {
      notes_stack0.add(pitch);
      for(int i=0;i<6;i++){
        voices[i].on = true;
        voices[i].note = pitch;
        setNote(i,pitch);
        if(notes_stack0.size()==1) //trigger on first note only
          keyOn(i);
      }

      break;
    }      
    case POLY6x1:
    {
      int index = -1;
      
      //attempts to find a free voice
      for(int i=0;i<6;i++)
        if(!voices[i].on){
          index = i;
          voices_order[voices_order_index] = i;
          break;
        }

      // no free voice, just kill the oldest
      if(index==-1)
        index = voices_order[voices_order_index];
      
      voices_order_index++;
      voices_order_index %=6;
      
      voices[index].on = true;
      voices[index].note = pitch;
      setNote(index,pitch);
      keyOn(index);   
      break;
    }
      
    case POLY3x2:
    {
      int index = -1;
      int concurrent_voices = 2;
      
      //attempts to find a free voice
      for(int i=0;i<6;i+=concurrent_voices)
        if(!voices[i].on){
          index = i;
          voices_order[voices_order_index] = i;
          break;
        }

      // no free voice, just kill the oldest
      if(index==-1)
        index = voices_order[voices_order_index];
      
      voices_order_index+=concurrent_voices;
      voices_order_index %=6;

      for(int i=0;i<concurrent_voices;i++){
        voices[index+i].on = true;
        voices[index+i].note = pitch;
        setNote(index+i,pitch);
        keyOn(index+i);   
      }      
      break;
    }
    case POLY2x3:
    {
      int index = -1;
      int concurrent_voices = 3;
      
      //attempts to find a free voice
      for(int i=0;i<6;i+=concurrent_voices)
        if(!voices[i].on){
          index = i;
          voices_order[voices_order_index] = i;
          break;
        }

      // no free voice, just kill the oldest
      if(index==-1)
        index = voices_order[voices_order_index];
      
      voices_order_index+=concurrent_voices;
      voices_order_index %=6;

      for(int i=0;i<concurrent_voices;i++){
        voices[index+i].on = true;
        voices[index+i].note = pitch;
        setNote(index+i,pitch);
        keyOn(index+i);   
      }      
      break;
      break;
    }
    case SPLIT_MONO1x3_MONO1x3:
      break;
      
    case SPLIT_MONO1x3_POLY3x1:
      break;
      
    case SPLIT_POLY3x1_MONO1x3:
      break;
      
    case SPLIT_POLY3x1_POLY3x1:
      break;
      
  }      


  
  /*
  //todo: only change slot operator levels
  //todo: instead for memcopy, hold tl and d1l in dedicated class vars
  uint8_t tl[16];
  uint8_t d1l_rr[16];
  float att = (float)(127 - velocity)/5.0f;
  //save total level
  memcpy(&tl, &(channels[0].TL), sizeof tl);
  memcpy(&d1l_rr, &(channels[0].D1L_RR), sizeof d1l_rr);

  for (int i = 0; i < 4; i++)
    if (operators[i]){
      setOperatorParameter(0, i, YM_OP_TL, tl[i*4] + att );
      //setOperatorParameter(0, i, YM_OP_D1L, (d1l_rr[i*4]/16) + att/8 );
    }

  //restore total level
  memcpy(&(channels[0].TL), &tl, sizeof tl);
  memcpy(&(channels[0].D1L_RR), &d1l_rr, sizeof d1l_rr);
  keyOn();
  */

}



void YM2612::noteOff(byte channel, byte pitch, byte velocity)
{
  UNUSED(channel);
  UNUSED(velocity);
  
  switch(playmode) {
  
    case MONO1x6:
      if(notes_stack0.size()>0){
        if( pitch == notes_stack0.get(notes_stack0.size()-1)){
          notes_stack0.pop();
          if(notes_stack0.size()>0){
            uint8_t previous = notes_stack0.get(notes_stack0.size()-1);
            for(int i=0;i<6;i++){
              voices[i].note = previous;
              setNote(i,previous);
              // dont trigger key on
            }           
          }           
        }else{
          // do nothing
          // only remove note from the stack
          for(int n = 0;n<notes_stack0.size();n++)
            if( pitch == notes_stack0.get(n)){
              notes_stack0.remove(n);
              break;
            }        
        }
      }

      // stack empty? trigger keyoff          
      if(notes_stack0.size()==0){
        for(int i = 0; i<6;i++){
          voices[i].on = false;
          keyOff(i);
        }        
      }

    
      break;
      
    case POLY6x1:
    
      for(int i = 0; i<6;i++)
        if(voices[i].on && voices[i].note == pitch){
          voices[i].on = false;
          keyOff(i);
        }    
      break;
      
    case POLY3x2:
      for(int i = 0; i<6;i++)
        if(voices[i].on && voices[i].note == pitch){
          voices[i].on = false;
          keyOff(i);
        }       
      break;
      
    case POLY2x3:
      for(int i = 0; i<6;i++)
        if(voices[i].on && voices[i].note == pitch){
          voices[i].on = false;
          keyOff(i);
        }       
      break;
      
    case SPLIT_MONO1x3_MONO1x3:
      break;
      
    case SPLIT_MONO1x3_POLY3x1:
      break;
      
    case SPLIT_POLY3x1_MONO1x3:
      break;
      
    case SPLIT_POLY3x1_POLY3x1:
      break;
      
  }  

  



}

void YM2612::pitchBend(byte channel, int bend){
  UNUSED(channel);  
  
  pitchBendValue = bend;
  for(int i = 0; i<6;i++)
  
    if(voices[i].on){
      float freqFrom = noteToFrequency(voices[i].note-2);;
      float freqTo = noteToFrequency(voices[i].note+2);
      setFrequency(i,map(bend,-8192,8191,freqFrom,freqTo));
    }  


  
}

float YM2612::noteToFrequency(uint8_t note){
    static float freq[] = {
      261.63f,   277.18f,   293.66f,   311.13f,   329.63f,   349.23f,   369.99f,   392.00f,   415.30f,   440.00f,   466.16f,   493.88f, 
    }; 
    static float multiplier[] = {
      0.03125f,   0.0625f,   0.125f,   0.25f,   0.5f,   1.0f,   2.0f,   4.0f,   8.0f,   16.0f,   32.0f, 
    }; 
  
  /*
    static float note2freq[] = {
        8.18f,     8.66f,     9.18f,     9.72f,    10.30f,    10.91f,    11.56f,    12.25f,    12.98f,    13.75f,    14.57f,    15.43f, 
       16.35f,    17.32f,    18.35f,    19.45f,    20.60f,    21.83f,    23.12f,    24.50f,    25.96f,    27.50f,    29.14f,    30.87f, 
       32.70f,    34.65f,    36.71f,    38.89f,    41.20f,    43.65f,    46.25f,    49.00f,    51.91f,    55.00f,    58.27f,    61.74f, 
       65.41f,    69.30f,    73.42f,    77.78f,    82.41f,    87.31f,    92.50f,    98.00f,   103.83f,   110.00f,   116.54f,   123.47f, 
      130.81f,   138.59f,   146.83f,   155.56f,   164.81f,   174.61f,   185.00f,   196.00f,   207.65f,   220.00f,   233.08f,   246.94f, 
      261.63f,   277.18f,   293.66f,   311.13f,   329.63f,   349.23f,   369.99f,   392.00f,   415.30f,   440.00f,   466.16f,   493.88f, 
      523.25f,   554.37f,   587.33f,   622.25f,   659.26f,   698.46f,   739.99f,   783.99f,   830.61f,   880.00f,   932.33f,   987.77f, 
     1046.50f,  1108.73f,  1174.66f,  1244.51f,  1318.51f,  1396.91f,  1479.98f,  1567.98f,  1661.22f,  1760.00f,  1864.66f,  1975.53f, 
     2093.00f,  2217.46f,  2349.32f,  2489.02f,  2637.02f,  2793.83f,  2959.96f,  3135.96f,  3322.44f,  3520.00f,  3729.31f,  3951.07f, 
     4186.01f,  4434.92f,  4698.64f,  4978.03f,  5274.04f,  5587.65f,  5919.91f,  6271.93f,  6644.88f,  7040.00f,  7458.62f,  7902.13f, 
     8372.02f,  8869.84f,  9397.27f,  9956.06f, 10548.08f, 11175.30f, 11839.82f, 12543.85f
    }; 
    */
  return freq[note%12]*multiplier[note/12];
}

void YM2612::setNote(uint8_t channel, uint8_t note){
    if(pitchBendValue==0){
      setFrequency(channel,noteToFrequency(note));
    }
    else{
      float freqFrom = noteToFrequency(note-2);;
      float freqTo = noteToFrequency(note+2);
      setFrequency(channel,map(pitchBendValue,-8192,8191,freqFrom,freqTo));
    }    
}

void YM2612::setFrequency(uint8_t chan, float frequency) {
  int block = 2;
  uint16_t freq;
  while (frequency >= 2048) {
    frequency /= 2;
    block++;
  }
  freq = (uint16_t)frequency;

  //setMasterParameter( YM_MA_OP_CHAN, 0xF0 + ((channel/3) * 4 + channel%3 ) );

  setRegister(chan/3, 0xA4+(chan%3), ((freq >> 8) & mask(3)) | ( ( block & mask(3) ) << 3)  ); // Set frequency
  setRegister(chan/3, 0xA0+(chan%3), freq);

}



void YM2612::keyOn(uint8_t chan) {
  setMasterParameter( YM_MA_OP_CHAN, 0xF0 + ((chan/3) * 4 + chan%3 ) );
}

void YM2612::keyOff(uint8_t chan) {
  setMasterParameter( YM_MA_OP_CHAN, 0x00 +((chan/3) * 4 + chan%3 ) );
}
    
void YM2612::update(){
}

