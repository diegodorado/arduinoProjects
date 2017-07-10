#ifndef AutoRange_h
#define AutoRange_h

#include "Arduino.h"

// library interface description
class AutoRange
{
  public:
  AutoRange(int pin, int dampMs, int minOutputValue,int maxOutputValue);
	int read();
  int value = -1;
  int outputValue;
  int minValue = 1023; // yes, these are correct
  int maxValue = 0;  // to let it autorange later
  void debug(); 
  private:

  float lerp(float v0, float v1, float t) {
    return v0 + t * (v1 - v0);
  }

  int minOutputValue;
  int maxOutputValue;

  int pin;
  long lastMillis = 0;
  int dampMs = 200;
};

#endif
