

#include "AutoRange.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

AutoRange::AutoRange(int pin, int dampMs, int minOutputValue,int maxOutputValue)
{

 this->pin = pin;
 this->dampMs = dampMs;
 this->minOutputValue = minOutputValue;
 this->maxOutputValue = maxOutputValue;
  
	pinMode(pin, INPUT);

}


int AutoRange::read() {
  long curMillis = millis();
  float t = (curMillis-lastMillis)/ ((float)dampMs);
  lastMillis = curMillis;

  // read the analog in value:
  int rawValue = analogRead(pin);

  //bypass lerp the first time
  if ( value == -1)
    value = rawValue;
  else
    value = lerp( value, rawValue, t);

  if ( value < minValue)
    minValue = value;

  if ( value > maxValue)
    maxValue = value;
    
  // map it to the range of the analog out:
  outputValue = map(value, minValue, maxValue, minOutputValue, maxOutputValue);
  return outputValue;

}

void AutoRange::debug(){
  Serial.print(minValue);
  Serial.print(" - \t - ");
  Serial.print(value);
  Serial.print(" - \t - ");
  Serial.print(maxValue);
  Serial.print(" - \t - ");
  Serial.print(minValue);
  Serial.print(" - \t::: - ");
  Serial.print(minOutputValue);
  Serial.print(" - \t - ");
  Serial.print(outputValue);
  Serial.print(" - \t - ");
  Serial.print(maxOutputValue);
  Serial.println("");
}

