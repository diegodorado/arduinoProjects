#ifndef YM2612_h
#define YM2612_h

#include "Arduino.h"

#include <inttypes.h>

#define YM_MASTER_ADDR (0x22)
#define YM_CHN_ADDR    (0x30)

typedef struct{
  union{
    uint8_t LFO; 
    struct{
      uint8_t RESERVED0:4;
      uint8_t LFO_E:1;
      uint8_t LFO_F:3;
    };
  };
  uint8_t RESERVED1:4;
  uint8_t TIMERA0;
  uint8_t TIMERA1;
  uint8_t TIMERB;
  union{
    uint8_t CHAN3_MODE_TIMERS; 
    struct{
      uint8_t CHAN3_MODE:2;
      uint8_t RESET_B:1;
      uint8_t RESET_A:1;
      uint8_t ENABLE_B:1;
      uint8_t ENABLE_A:1;
      uint8_t LOAD_B:1;
      uint8_t LOAD_A:1;
    };
  };
  union{
    uint8_t OP_CHAN; 
    struct{
      uint8_t OPERATOR:4;
      uint8_t RESERVED2:1;
      uint8_t CHANNEL:3;
    };
  };
  uint8_t RESERVED3;
  uint8_t DAC;
  union{
    uint8_t _DAC_EN; 
    struct{
      uint8_t DAC_EN:1;
      uint8_t RESERVED4:7;
    };
  };
} YM_MASTER_t;

typedef union{
  uint8_t data;
  struct{
    uint8_t RESERVED:1;
    uint8_t DT1:3;
    uint8_t MUL:4;
  };
} DT1_MUL_t;

typedef union{
  uint8_t data;
  struct{
    uint8_t TL:7;
    uint8_t RESERVED:1;
  };
} TL_t;

typedef union{
  uint8_t data; 
  struct{
    uint8_t RS:2;//rate scaling 
    uint8_t RESERVED:1;
    uint8_t AR:5;// attack rate
  };
} RS_AR_t;

typedef union{
  uint8_t data;
  struct{
    uint8_t AM:1; //amp modulation 
    uint8_t RESERVED:2;
    uint8_t D1R:5; // first decay rate
  };
} AM_D1R_t;

typedef union{
  uint8_t data; 
  struct{
    uint8_t RESERVED:3;
    uint8_t D2R:5;//secondary decay rate
  };
} D2R_t;

typedef union{
  uint8_t data; 
  struct{
    uint8_t D1L:4;
    uint8_t RR:4;
  };
} D1L_RR_t;

typedef union{
  uint8_t data;
  struct{
    uint8_t RESERVED:2;
    uint8_t BLOCK:3;
    uint8_t FREQ_MSB:3;
  };
} BLOCK_FREQ_MSB_t;


typedef union{
  uint8_t data;
  struct{
    uint8_t RESERVED:2;
    uint8_t FB:3;
    uint8_t ALG:3;
  };
} FB_ALG_t;


typedef union{
  uint8_t data;
  struct{
    uint8_t L:1;
    uint8_t R:1;
    uint8_t AMS:2;
    uint8_t RESERVED:1;
    uint8_t FMS:3;
  };
} L_R_AMS_FMS_t;


typedef struct
{
  DT1_MUL_t DT1_MUL[16];
  TL_t TL[16];
  RS_AR_t RS_AR[16];
  AM_D1R_t AM_D1R[16];
  D2R_t D2R[16];
  D1L_RR_t D1L_RR[16];
  uint8_t SSG_EG[16]; //proprietary and should be set to zero.  
  uint8_t FREQ_LSB[4];
  BLOCK_FREQ_MSB_t BLOCK_FREQ_MSB[4];
  uint8_t CHAN3_FREQ_LSB[4];
  BLOCK_FREQ_MSB_t CHAN3_BLOCK_FREQ_MSB[4];
  FB_ALG_t FB_ALG[4];
  L_R_AMS_FMS_t L_R_AMS_FMS[4];
} ThreeChannels_t;

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
    void setLFO(int enable, int frequency);
    void setFrequency(int frequency, int block);
    void setSupplementaryFrequency(int frequency, int block);
    void setFeedback(int value); 
    void setAlgorithm(int value); 
    void setAMS(int value); 
    void setFMS(int value); 
    void setStereo(int left, int right); 
    void setAmplitudeModulation(int value); 
    void setAttackRate(int value); 
    void setDecayRate(int value);
    void setSustainRate(int value);
    void setReleaseRate(int value);
    void setTotalLevel(int value);
    void setSustainLevel(int value);
    void setMultiply(int value);
    void setDetune(int value);
    void setRateScaling(int value); 
    void setOperator(int op, int value);
    
  private:
    uint8_t channel;
    bool operators[4];
    YM_MASTER_t master;
    ThreeChannels_t channels[2];    
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
    void setreg(uint8_t reg, uint8_t data);
    void setRegPart0(uint8_t reg, uint8_t data);
    void setRegPart1(uint8_t reg, uint8_t data);
    void setRegPart(uint8_t part, uint8_t reg, uint8_t data);
    void setRegValue(int value);
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






void setup_operators(){
  
  operator1.DT1 = 7;
  operator1.MUL = 1;
  operator1.TL = 35;
  operator1.RS = 2;
  operator1.AR = 31;
  operator1.AM = 0;
  operator1.D1R = 5;
  operator1.D2R = 2;
  operator1.D1L = 1;
  operator1.RR = 1;
  operator1._SSG_EG = 0;

  setreg( YM_CHN1_ADDR + offsetof(operator_t, _DT1_MUL), operator1._DT1_MUL);
  setreg( YM_CHN1_ADDR + offsetof(operator_t, _TL), operator1._TL);
  setreg( YM_CHN1_ADDR + offsetof(operator_t, _RS_AR), operator1._RS_AR);
  setreg( YM_CHN1_ADDR + offsetof(operator_t, _AM_D1R), operator1._AM_D1R);
  setreg( YM_CHN1_ADDR + offsetof(operator_t, _D2R), operator1._D2R);
  setreg( YM_CHN1_ADDR + offsetof(operator_t, _D1L_RR), operator1._D1L_RR);
  setreg( YM_CHN1_ADDR + offsetof(operator_t, _SSG_EG), operator1._SSG_EG);

  
}




void setAlgorithm(int value){
}


*/
