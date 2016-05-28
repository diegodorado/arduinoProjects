#ifndef YM2612_h
#define YM2612_h

#include "Arduino.h"

#include <inttypes.h>

#define YM_MASTER_ADDR (0x22)
#define YM_CHN_ADDR    (0x30)

#define YM_MA_LFO_E  offsetof(Master_t,LFO), 1, 3
#define YM_MA_LFO_F  offsetof(Master_t,LFO), 3, 0
#define YM_MA_CH3_M  offsetof(Master_t,CHAN3_MODE_TIMERS), 2, 6

#define YM_OP_AR  offsetof(Channels_t,RS_AR), 5, 0
#define YM_OP_AM  offsetof(Channels_t,AM_D1R), 1, 7
#define YM_OP_D1R offsetof(Channels_t,AM_D1R), 5, 0
#define YM_OP_D2R offsetof(Channels_t,D2R), 5, 0
#define YM_OP_RR  offsetof(Channels_t,D1L_RR), 4, 4
#define YM_OP_TL  offsetof(Channels_t,TL), 7, 0
#define YM_OP_SSG_EG offsetof(Channels_t,SSG_EG), 8, 0
#define YM_OP_D1L offsetof(Channels_t,D1L_RR), 4, 4
#define YM_OP_MUL offsetof(Channels_t,DT1_MUL), 4, 0
#define YM_OP_DT1 offsetof(Channels_t,DT1_MUL), 3, 4
#define YM_OP_RS  offsetof(Channels_t,RS_AR), 2, 6

#define YM_CH_FB  offsetof(Channels_t,FB_ALG), 3, 3
#define YM_CH_ALG offsetof(Channels_t,FB_ALG), 3, 0
#define YM_CH_L_R offsetof(Channels_t,L_R_AMS_FMS), 2, 6
#define YM_CH_AMS offsetof(Channels_t,L_R_AMS_FMS), 3, 3
#define YM_CH_FMS offsetof(Channels_t,L_R_AMS_FMS), 2, 0

#define mask(s) (~(~0<<s))


typedef struct{
  uint8_t LFO; 
  uint8_t RESERVED0;
  uint8_t TIMERA0;
  uint8_t TIMERA1;
  uint8_t TIMERB;
  uint8_t CHAN3_MODE_TIMERS; 
  uint8_t OP_CHAN; 
  uint8_t RESERVED1;
  uint8_t DAC;
  uint8_t DAC_EN;
} Master_t;



typedef struct
{
  uint8_t DT1_MUL[16];
  uint8_t TL[16];
  uint8_t RS_AR[16];
  uint8_t AM_D1R[16];
  uint8_t D2R[16];
  uint8_t D1L_RR[16];
  uint8_t SSG_EG[16]; //proprietary and should be set to zero.  
  uint8_t FREQ[16];
  uint8_t FB_ALG[4];
  uint8_t L_R_AMS_FMS[4];
} Channels_t;

class YM2612
{
  public:
    void setup(uint8_t ic_pin,
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
               uint8_t data7_pin);
    void initialize();
    void keyOn();
    void keyOff();
    void setChannel(int value) {channel = value;}
    void setUnison(int value) { unison = (value>0);}
    void setOperator(int op, int value) {operators[op] = (value>0);}
    void setLFO(int enable, int frequency);
    void setFrequency(int frequency, int block);
    void setSupplementaryFrequency(int frequency, int block);
    
    //master params
    void setLFO_Enable          (int value) { setMasterParameter( YM_CH_FB, value);}
    void setLFO_Frequency       (int value) { setMasterParameter( YM_MA_LFO_F, value);}
    void setChan3Mode           (int value) { setMasterParameter( YM_MA_CH3_M, value);}
    
    //channel params
    void setFeedback            (int value) { setMasterParameter( YM_CH_FB, value);}
    void setAlgorithm           (int value) { setChannelParameter( YM_CH_ALG, value);}
    void setStereo              (int l, int r) { setChannelParameter( YM_CH_L_R, (l<<1)&r);}
    void setAMS                 (int value) { setChannelParameter( YM_CH_AMS, value);}
    void setFMS                 (int value) { setChannelParameter( YM_CH_FMS, value);}
    //operator params
    void setAmplitudeModulation (int value) { setOperatorParameter( YM_OP_AM, value);}
    void setAttackRate          (int value) { setOperatorParameter( YM_OP_AR, value);}
    void setDecayRate           (int value) { setOperatorParameter( YM_OP_D1R, value);}
    void setSustainRate         (int value) { setOperatorParameter( YM_OP_D2R, value);}
    void setReleaseRate         (int value) { setOperatorParameter( YM_OP_RR, value);}
    void setTotalLevel          (int value) { setOperatorParameter( YM_OP_TL, value);}
    void setSustainLevel        (int value) { setOperatorParameter( YM_OP_D1L, value);}
    void setMultiply            (int value) { setOperatorParameter( YM_OP_MUL, value);}
    void setDetune              (int value) { setOperatorParameter( YM_OP_DT1, value);}
    void setRateScaling         (int value) { setOperatorParameter( YM_OP_RS, value);}
    void setSSG_EG              (int value) { setOperatorParameter( YM_OP_SSG_EG, value);}
    
  private:
    uint8_t channel;
    bool operators[4];
    bool unison;
    Master_t master;
    Channels_t channels[2];    
    uint8_t ic_pin;
    uint8_t cs_pin;
    uint8_t wr_pin;
    uint8_t rd_pin;
    uint8_t a0_pin;
    uint8_t a1_pin;
    uint8_t mc_pin;
    uint8_t data0_pin;
    uint8_t data1_pin;
    uint8_t data2_pin;
    uint8_t data3_pin;
    uint8_t data4_pin;
    uint8_t data5_pin;
    uint8_t data6_pin;
    uint8_t data7_pin;
    uint8_t channelPart();
    uint8_t channelOffset();
    void write_ym(uint8_t data);
    void setRegPart0(uint8_t reg, uint8_t data);
    void setRegPart1(uint8_t reg, uint8_t data);
    void setRegPart(uint8_t part, uint8_t reg, uint8_t data);
    void setMasterParameter(int reg_offset, int val_size, int val_shift,int val);
    void setChannelParameter(int chan,int reg_offset, int val_size, int val_shift,int val);
    void setChannelParameter(int reg_offset, int val_size, int val_shift,int val);
    void setOperatorParameter(int chan,int reg_offset, int val_size, int val_shift,int val);
    void setOperatorParameter(int reg_offset, int val_size, int val_shift,int val);
    void create_piano();
};


#endif




/*

from http://www.smspower.org/maxim/Documents/YM2612#reg27

Part I memory map
=================

+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| D7   | D6                                 | D5  | D4                      | D3      | D2         | D1                                 | D0           |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 22H  |                                    |     |                         |         | LFO enable | LFO frequency                      |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 24H  | Timer A MSBs                       |     |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 25H  |                                    |     |                         |         |            |                                    | Timer A LSBs |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 26H  | Timer B                            |     |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 27H  | Ch3 mode                           |     | Reset B                 | Reset A | Enable B   | Enable A                           | Load B       | Load A |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 28H  | Operator                           |     |                         |         |            | Channel                            |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 29H  |                                    |     |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 2AH  | DAC                                |     |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 2BH  | DAC en                             |     |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
|      |                                    |     |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 30H+ |                                    | DT1 |                         |         | MUL        |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 40H+ |                                    | TL  |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 50H+ | RS                                 |     |                         | AR      |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 60H+ | AM                                 |     |                         | D1R     |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 70H+ |                                    |     |                         | D2R     |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 80H+ | D1L                                |     |                         |         | RR         |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| 90H+ |                                    |     |                         |         | SSG-EG     |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
|      |                                    |     |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| A0H+ | Frequency number LSB               |     |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| A4H+ |                                    |     | Block                   |         |            | Frequency Number MSB               |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| A8H+ | Ch3 supplementary frequency number |     |                         |         |            |                                    |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| ACH+ |                                    |     | Ch3 supplementary block |         |            | Ch3 supplementary frequency number |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| B0H+ |                                    |     | Feedback                |         |            | Algorithm                          |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+
| B4H+ | L                                  | R   | AMS                     |         |            | FMS                                |              |        |
+------+------------------------------------+-----+-------------------------+---------+------------+------------------------------------+--------------+--------+


*/
