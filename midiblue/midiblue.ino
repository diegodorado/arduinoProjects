#include <MIDI.h>
#include <SoftwareSerial.h>
SoftwareSerial softSerial(2,3);
struct BleSettings : public midi::DefaultSettings
{
  static const long BaudRate = 9600;
};
//MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, midiB, BleSettings);
MIDI_CREATE_CUSTOM_INSTANCE(SoftwareSerial, softSerial, midiB, BleSettings);
 
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  digitalWrite(LED_BUILTIN,HIGH);
 }

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    digitalWrite(LED_BUILTIN,LOW);

}


void setup()
{  
    pinMode(LED_BUILTIN, OUTPUT);

    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
    midiB.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
    midiB.setHandleNoteOff(handleNoteOff);

    // Initiate MIDI communications, listen to all channels
    midiB.begin(MIDI_CHANNEL_OMNI);
}

void loop()
{
    // Call MIDI.read the fastest you can for real-time performance.
    midiB.read();
}
