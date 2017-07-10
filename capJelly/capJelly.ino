#include <CapacitiveSensor.h>

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

void setup()                    
{
   Serial.begin(9600);
}

void loop()                    
{
  
    long total1 =  cs_4_2.capacitiveSensor(50);
    byte m = map(total1, 1, 1 000, 0, 127);
    cc(m);
    delay(1);                             // arbitrary delay to limit data to serial port 
}


void cc(byte val) {
  Serial.write(0xB0);
  Serial.write(0x10);
  Serial.write(( (byte)val) & 0x3F);
}


//  plays a MIDI note.
void bend(unsigned int val) {
  Serial.write(0xE0);
  //Serial.write(0x00);
  //Serial.write(0x40);
  Serial.write(( (byte)val) & 0x3F); //lsb
  Serial.write( ((byte)(val>>7)) & 0x3F); //msb
}


