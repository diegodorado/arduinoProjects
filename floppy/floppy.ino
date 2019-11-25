#include <MIDI.h>

struct MidiSettings : public midi::DefaultSettings
{
    static const long BaudRate = 9600;
};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, MidiSettings);

#include <TimerOne.h>

#define RESOLUTION 40 //Microsecond resolution for notes
byte MAX_POSITION = 158;
  
byte currentPosition = 0;
int currentState = LOW;
int currentStateDir = LOW;
unsigned int currentPeriod = 0;
unsigned int currentTick = 0;
byte keys[128]= {0};


unsigned int microPeriods[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        30578, 28861, 27242, 25713, 24270, 22909, 21622, 20409, 19263, 18182, 17161, 16198, //C1 - B1
        15289, 14436, 13621, 12856, 12135, 11454, 10811, 10205, 9632, 9091, 8581, 8099, //C2 - B2
        7645, 7218, 6811, 6428, 6068, 5727, 5406, 5103, 4816, 4546, 4291, 4050, //C3 - B3
        3823, 3609, 3406, 3214, 3034, 2864, 2703, 2552, 2408, 2273, 2146, 2025, //C4 - B4
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };


void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    currentPeriod = microPeriods[pitch] / (RESOLUTION * 2);
    keys[pitch] = 1;
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    keys[pitch] = 0;
    for(int i=0;i<128;i++){
      //quit if key is still on
      if(keys[i]>0)
        return;
    }
    currentPeriod = 0;
    
}

void setup(){
  pinMode(2, OUTPUT); // Step control 1
  pinMode(3, OUTPUT); // Direction 1
  pinMode(13, OUTPUT);

    
  resetAll();
  delay(2000);

  Timer1.initialize(RESOLUTION); // Set up a timer at the defined resolution
  Timer1.attachInterrupt(tick); // Attach the tick function

  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
}

long prevMillis = 0;
int note = 25;

void loop(){
  MIDI.read();
  
  long currMillis = millis();
  if (currMillis -prevMillis > 300){

    //currentPeriod = microPeriods[note] / (RESOLUTION * 2);

    note++;
    if(note > 24+4*12)
      note = 24;
    
    prevMillis = currMillis;
  }
}

void tick()
{
  if (currentPeriod>0){
    currentTick++;
    if (currentTick >= currentPeriod){
      togglePin();
      currentTick=0;
    }
  }
  
}

void togglePin() {
  
  //Switch directions if end has been reached
  if (currentPosition >= MAX_POSITION) {
    currentStateDir = HIGH;
    digitalWrite(3,HIGH);
  } 
  else if (currentPosition <= 0) {
    currentStateDir = LOW;
    digitalWrite(3,LOW);
  }
  
    //Update currentPosition
  if (currentStateDir == HIGH){
    currentPosition--;
  } 
  else {
    currentPosition++;
  }
  
  //Pulse the control pin
  digitalWrite(2,currentState);
  currentState = !currentState;
}


void resetAll(){
  
  digitalWrite(3,HIGH); // Go in reverse
  for (byte s=0;s<250;s++){ // For max drive's position
    digitalWrite(2,HIGH);
    digitalWrite(2,LOW);
    delay(5);
  }
  
  currentPosition = 0;
  digitalWrite(3,LOW);
  currentState = 0;
  currentStateDir = LOW;
  
}



