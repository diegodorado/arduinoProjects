#include "YM2612.h"

//awful...but
#define addr_offset(REG) (YM_CHN_ADDR + offsetof(ThreeChannels_t, REG))
#define reg_val(REG, PROP) for(int i=0;i<4;i++){ \
    int offset = i*4 + channelOffset(); \
    if(operators[i]){ \
        channels[channelPart()].REG[offset].PROP = value;\
        setRegPart(channelPart(), addr_offset(REG) + offset, channels[channelPart()].REG[offset].data);  \
    } \
  }

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

  channel =1;
  operators[0] = true;
  operators[1] = true;
  operators[2] = true;
  operators[3] = true;
  create_piano();
  setreg(0xB0, 0x07); // Feedback/algorithm

}

void YM2612::setFrequency(int frequency, int block){
  setreg(0xA4, ((frequency>>8) & 0B00000111) | (0B00111000 & (block<<3) ) ); // Set frequency
  setreg(0xA0, frequency);
}

void YM2612::keyOn(){
  setreg(0x28, 0xF0); // Key on
}

void YM2612::keyOff(){
  setreg(0x28, 0x00); // Key off
}


void YM2612::create_piano() {
  /* YM2612 Test code */ 
  setreg(0x22, 0x00); // LFO off
  setreg(0x27, 0x00); // Note off (channel 0)
  setreg(0x28, 0x01); // Note off (channel 1)
  setreg(0x28, 0x02); // Note off (channel 2)
  setreg(0x28, 0x04); // Note off (channel 3)
  setreg(0x28, 0x05); // Note off (channel 4)
  setreg(0x28, 0x06); // Note off (channel 5)
  setreg(0x2B, 0x00); // DAC off
  setreg(0x30, 0x71); //
  setreg(0x34, 0x0D); //
  setreg(0x38, 0x33); //
  setreg(0x3C, 0x01); // DT1/MUL
  setreg(0x40, 0x23); //
  setreg(0x44, 0x2D); //
  setreg(0x48, 0x26); //
  setreg(0x4C, 0x00); // Total level
  setreg(0x50, 0x5F); //
  setreg(0x54, 0x99); //
  setreg(0x58, 0x5F); //
  setreg(0x5C, 0x94); // RS/AR 
  setreg(0x60, 0x05); //
  setreg(0x64, 0x05); //
  setreg(0x68, 0x05); //
  setreg(0x6C, 0x07); // AM/D1R
  setreg(0x70, 0x02); //
  setreg(0x74, 0x02); //
  setreg(0x78, 0x02); //
  setreg(0x7C, 0x02); // D2R
  setreg(0x80, 0x11); //
  setreg(0x84, 0x11); //
  setreg(0x88, 0x11); //
  setreg(0x8C, 0xA6); // D1L/RR
  setreg(0x90, 0x00); //
  setreg(0x94, 0x00); //
  setreg(0x98, 0x00); //
  setreg(0x9C, 0x00); // Proprietary
  setreg(0xB0, 0x32); // Feedback/algorithm
  setreg(0xB4, 0xC0); // Both speakers on
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

void YM2612::setreg(uint8_t reg, uint8_t data) {
  setRegPart0(reg,data);
}


void YM2612::setRegPart0(uint8_t reg, uint8_t data) {
  digitalWrite(a0_pin, LOW);
  digitalWrite(a1_pin, LOW);
  write_ym(reg);
  digitalWrite(a0_pin, HIGH);
  write_ym(data);
}

void YM2612::setRegPart1(uint8_t reg, uint8_t data) {
  digitalWrite(a0_pin, HIGH);
  digitalWrite(a1_pin, LOW);
  write_ym(reg);
  digitalWrite(a1_pin, HIGH);
  write_ym(data);
}

void YM2612::setRegPart(uint8_t part, uint8_t reg, uint8_t data){
  if(part==0)
    setRegPart0(reg, data);
  else
    setRegPart1(reg, data);
}



void YM2612::setAttackRate(int value){  
  reg_val(RS_AR, AR)
}

void YM2612::setDecayRate(int value){
  reg_val(AM_D1R, D1R)
}

void YM2612::setSustainRate(int value){
  reg_val(D2R, D2R)
}

void YM2612::setReleaseRate(int value){
  reg_val(D1L_RR, RR)
}

void YM2612::setTotalLevel(int value){
  //reg_val(TL, TL)

  channels[0].TL[0].TL = value;
  channels[0].TL[4].TL = value;
  channels[0].TL[8].TL = value;
  channels[0].TL[12].TL = value;
  setRegPart(0, addr_offset(TL) + 0, channels[0].TL[0].data);  
  setRegPart(0, addr_offset(TL) + 4, channels[0].TL[4].data);  
  setRegPart(0, addr_offset(TL) + 8, channels[0].TL[8].data);  
  setRegPart(0, addr_offset(TL) + 12, channels[0].TL[12].data);  
  
  //setreg(0x40, value); //
  //setreg(0x44, value); //
  //setreg(0x48, value); //
  //setreg(0x4C, value); // Total level
}

void YM2612::setSustainLevel(int value){
  reg_val(D1L_RR, D1L)
}

void YM2612::setMultiply(int value){
  reg_val(DT1_MUL, MUL)
}

void YM2612::setDetune(int value){
  reg_val(DT1_MUL, DT1)
}

void YM2612::setRateScaling(int value){
  reg_val(RS_AR, RS)
}

void YM2612::setOperator(int op, int value){
  operators[op] = (value>0);
}

uint8_t YM2612::channelPart(){
  return (channel<=3) ? 0 : 1;
}
  
uint8_t YM2612::channelOffset(){
  switch(channel) {
   case 1:
   case 4:
      return 0;
   case 2:
   case 5:
      return 1;
   case 3:
   case 6:
      return 2;
  }
}
