#include <Servo.h>
#include <MIDI.h>
struct BleSettings : public midi::DefaultSettings
{
  static const long BaudRate = 9600;
};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, midiB, BleSettings);

Servo servo0;
Servo servo1;

 
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if(pitch=60)
    servo0.write(55);
  //else
  //  servo1.write(20);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  if(pitch=60)
    servo0.write(35);
  //else
  //  servo1.write(0);
}

void setup() {
  servo0.attach(11);
  servo1.attach(9);
  delay(500);
  servo0.write(0);
  servo1.write(0);
  delay(500);
  servo0.write(20);
  servo1.write(20);
  midiB.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
  midiB.setHandleNoteOff(handleNoteOff);
  midiB.begin(MIDI_CHANNEL_OMNI);

}

void loop() {
  // Call MIDI.read the fastest you can for real-time performance.
  midiB.read();
}

