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


  create_piano();

}

void YM2612::setFrequency(int frequency, int block){
  setRegPart0(0xA4, ((frequency>>8) & 0B00000111) | (0B00111000 & (block<<3) ) ); // Set frequency
  setRegPart0(0xA0, frequency);

  setRegPart0(0xA5, ((frequency>>8) & 0B00000111) | (0B00111000 & (block<<3) ) ); // Set frequency
  setRegPart0(0xA1, frequency);

  setRegPart0(0xA6, ((frequency>>8) & 0B00000111) | (0B00111000 & (block<<3) ) ); // Set frequency
  setRegPart0(0xA2, frequency);

  setRegPart1(0xA4, ((frequency>>8) & 0B00000111) | (0B00111000 & (block<<3) ) ); // Set frequency
  setRegPart1(0xA0, frequency);

  setRegPart1(0xA5, ((frequency>>8) & 0B00000111) | (0B00111000 & (block<<3) ) ); // Set frequency
  setRegPart1(0xA1, frequency);

  setRegPart1(0xA6, ((frequency>>8) & 0B00000111) | (0B00111000 & (block<<3) ) ); // Set frequency
  setRegPart1(0xA2, frequency);
  
}

void YM2612::keyOn(){
  setRegPart0(0x28, 0xF0); // Key on
  setRegPart0(0x28, 0xF1); // Key on
  setRegPart0(0x28, 0xF2); // Key on
  setRegPart0(0x28, 0xF4); // Key on
  setRegPart0(0x28, 0xF5); // Key on
  setRegPart0(0x28, 0xF6); // Key on
}

void YM2612::keyOff(){
  setRegPart0(0x28, 0x00); // Key off
  setRegPart0(0x28, 0x01); // Key off
  setRegPart0(0x28, 0x02); // Key off
  setRegPart0(0x28, 0x04); // Key off
  setRegPart0(0x28, 0x05); // Key off
  setRegPart0(0x28, 0x06); // Key off
}

/*
void create_piano2() {

  unison = true;
  channel =0;
  operators[0] = true;
  operators[1] = true;
  operators[2] = true;
  operators[3] = true;
  
  setLFO_Enable(0);
  keyOff();
  setRegPart0(0x2B, 0x00); // DAC off

  setLFO_Enable(1);
  setLFO_Frequency(2);      
  setChan3Mode(0);
    
  //channel params
  setFeedback(1);
  setAlgorithm(7);
  setStereo(1,1);
  setAMS(3);
  setFMS(2);
  //operator params
  setAmplitudeModulation(1);
  setAttackRate(15);
  setDecayRate(15);
  setSustainRate(15);
  setReleaseRate(5);
  setTotalLevel(0);
  setSustainLevel(8);
  setMultiply(2);
  setDetune(3);
  setRateScaling(1);
  setSSG_EG(0);
  keyOff();
}
*/
void YM2612::create_piano() {

  unison = false;
  channel =0;
  operators[0] = true;
  operators[1] = true;
  operators[2] = true;
  operators[3] = true;
  
  setRegPart0(0x22, 0x00); // LFO off
  setRegPart0(0x27, 0x00); // Note off (channel 0)
  setRegPart0(0x28, 0x01); // Note off (channel 1)
  setRegPart0(0x28, 0x02); // Note off (channel 2)
  setRegPart0(0x28, 0x04); // Note off (channel 3)
  setRegPart0(0x28, 0x05); // Note off (channel 4)
  setRegPart0(0x28, 0x06); // Note off (channel 5)
  setRegPart0(0x2B, 0x00); // DAC off
  setRegPart0(0x30, 0x71); //
  setRegPart0(0x34, 0x0D); //
  setRegPart0(0x38, 0x33); //
  setRegPart0(0x3C, 0x01); // DT1/MUL
  setRegPart0(0x40, 0x23); //
  setRegPart0(0x44, 0x2D); //
  setRegPart0(0x48, 0x26); //
  setRegPart0(0x4C, 0x00); // Total level
  setRegPart0(0x50, 0x5F); //
  setRegPart0(0x54, 0x99); //
  setRegPart0(0x58, 0x5F); //
  setRegPart0(0x5C, 0x94); // RS/AR 
  setRegPart0(0x60, 0x05); //
  setRegPart0(0x64, 0x05); //
  setRegPart0(0x68, 0x05); //
  setRegPart0(0x6C, 0x07); // AM/D1R
  setRegPart0(0x70, 0x02); //
  setRegPart0(0x74, 0x02); //
  setRegPart0(0x78, 0x02); //
  setRegPart0(0x7C, 0x02); // D2R
  setRegPart0(0x80, 0x11); //
  setRegPart0(0x84, 0x11); //
  setRegPart0(0x88, 0x11); //
  setRegPart0(0x8C, 0xA6); // D1L/RR
  setRegPart0(0x90, 0x00); //
  setRegPart0(0x94, 0x00); //
  setRegPart0(0x98, 0x00); //
  setRegPart0(0x9C, 0x00); // Proprietary
  setRegPart0(0xB0, 0x32); // Feedback/algorithm
  setRegPart0(0xB4, 0xC0); // Both speakers on


  setRegPart1(0x30, 0x71); //
  setRegPart1(0x34, 0x0D); //
  setRegPart1(0x38, 0x33); //
  setRegPart1(0x3C, 0x01); // DT1/MUL
  setRegPart1(0x40, 0x23); //
  setRegPart1(0x44, 0x2D); //
  setRegPart1(0x48, 0x26); //
  setRegPart1(0x4C, 0x00); // Total level
  setRegPart1(0x50, 0x5F); //
  setRegPart1(0x54, 0x99); //
  setRegPart1(0x58, 0x5F); //
  setRegPart1(0x5C, 0x94); // RS/AR 
  setRegPart1(0x60, 0x05); //
  setRegPart1(0x64, 0x05); //
  setRegPart1(0x68, 0x05); //
  setRegPart1(0x6C, 0x07); // AM/D1R
  setRegPart1(0x70, 0x02); //
  setRegPart1(0x74, 0x02); //
  setRegPart1(0x78, 0x02); //
  setRegPart1(0x7C, 0x02); // D2R
  setRegPart1(0x80, 0x11); //
  setRegPart1(0x84, 0x11); //
  setRegPart1(0x88, 0x11); //
  setRegPart1(0x8C, 0xA6); // D1L/RR
  setRegPart1(0x90, 0x00); //
  setRegPart1(0x94, 0x00); //
  setRegPart1(0x98, 0x00); //
  setRegPart1(0x9C, 0x00); // Proprietary
  setRegPart1(0xB0, 0x32); // Feedback/algorithm
  setRegPart1(0xB4, 0xC0); // Both speakers on
  
  keyOff();
}

void YM2612::write_ym(uint8_t data) {
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


void YM2612::setRegPart0(uint8_t reg, uint8_t data) {
  digitalWrite(a1_pin, LOW);
  digitalWrite(a0_pin, LOW);
  write_ym(reg);
  digitalWrite(a0_pin, HIGH);
  write_ym(data);
}

void YM2612::setRegPart1(uint8_t reg, uint8_t data) {
  digitalWrite(a1_pin, HIGH);
  digitalWrite(a0_pin, LOW);
  write_ym(reg);
  digitalWrite(a0_pin, HIGH);
  write_ym(data);
}

void YM2612::setRegPart(uint8_t part, uint8_t reg, uint8_t data){
  if(part==0)
    setRegPart0(reg, data);
  else
    setRegPart1(reg, data);
}

void YM2612::setMasterParameter(int reg_offset, int val_size, int val_shift,int val){
  uint8_t* p = (uint8_t *) ( &master) + reg_offset;

  *(p) &= ~(mask(val_size)<<val_shift); //clean
  *(p) |= ((mask(val_size) & val)<<val_shift); //write
  setRegPart(0, YM_MASTER_ADDR + reg_offset, *(p));
  
}

void YM2612::setChannelParameter(int reg_offset, int val_size, int val_shift,int val){

    if(unison){
      for(int i=0;i<6;i++)
        setChannelParameter(i, reg_offset, val_size, val_shift, val);   
    }else{
      setChannelParameter(channel, reg_offset, val_size, val_shift, val);   
    }
  
}

void YM2612::setChannelParameter(int chan, int reg_offset, int val_size, int val_shift,int val){

  int channel_part = chan/3;
  uint8_t* p = (uint8_t *) ( &channels[channel_part]) + reg_offset;
  int channel_offset = chan%3; //which of the 12 registers

  *(p+channel_offset) &= ~(mask(val_size)<<val_shift); //clean
  *(p+channel_offset) |= ((mask(val_size) & val)<<val_shift); //write
  setRegPart(channel_part, YM_CHN_ADDR + reg_offset + channel_offset, *(p+channel_offset));
  
}


void YM2612::setOperatorParameter(int reg_offset, int val_size, int val_shift,int val){

    if(unison){
      for(int i=0;i<6;i++)
        setOperatorParameter(i, reg_offset, val_size, val_shift, val);   
    }else{
      setOperatorParameter(channel, reg_offset, val_size, val_shift, val);   
    }
  
}


void YM2612::setOperatorParameter(int chan, int reg_offset, int val_size, int val_shift,int val){

  int channel_part = chan/3;
  uint8_t* p = (uint8_t *) ( &channels[channel_part]) + reg_offset;
  
  for(int i=0;i<4;i++){ 
    int op_offset = chan%3 + i*4; //which of the 12 registers
    if(operators[i]){
      *(p+op_offset) &= ~(mask(val_size)<<val_shift); //clean
      *(p+op_offset) |= ((mask(val_size) & val)<<val_shift); //write
      setRegPart(channel_part, YM_CHN_ADDR + reg_offset + op_offset, *(p+op_offset));      
    }
  }
  
}

