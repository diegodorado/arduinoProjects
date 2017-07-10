// set midi channel to be sent
int channel = 10;
// set the octave height
int octave = 2;

int notas[] = {0,1,2,3,4,5,6,7};


void setup() {

  Serial.begin(9600);

}

void loop() {

  for(int i=0;i<8;i++){
    noteOn(notas[i], 100);
    delay(1500);
    noteOff(notas[i]);
    delay(500);
  }

}




//  plays a MIDI note.
void noteOn(int pitch, int velocity) {
  int offset= 24; // C1
  int note = offset + pitch + octave * 12;
  Serial.write(0x90 + channel - 1 );
  Serial.write(note);
  Serial.write(velocity);
}

void noteOff(int pitch) {
  noteOn(pitch, 0);
}


