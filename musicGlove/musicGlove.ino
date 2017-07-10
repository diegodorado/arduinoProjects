int pines[] = {5,6,8,11,12};
int estadoPines[] = {1,1,1,1,1};
int antirebotes[] = {0,0,0,0,0};
int notas[] = {60,62,64,65,67};


void setup() {

  for(int i=0;i<5;i++){
    pinMode(pines[i], INPUT_PULLUP);
  }
  
  Serial.begin(9600);

}

void loop() {

  for(int i=0;i<5;i++){
    int nuevoEstadoPin = digitalRead(pines[i]);
    
    if (estadoPines[i] != nuevoEstadoPin ){
      antirebotes[i] = antirebotes[i] + 1;
  
      if(antirebotes[i] > 200 ){
        cambiarEstadoPin(i,nuevoEstadoPin);
      }
    }else{
        antirebotes[i]= 0;
    }
  }

}


void cambiarEstadoPin(int pin, int nuevoEstado) {
  estadoPines[pin] = nuevoEstado;
  antirebotes[pin] = 0;
  if(estadoPines[pin] == 0){
    noteOn(notas[pin], 100);
  }else{
    noteOff(notas[pin]);
  }
}



//  plays a MIDI note.
void noteOn(int pitch, int velocity) {
  Serial.write(0x90);
  Serial.write(pitch);
  Serial.write(velocity);
}

void noteOff(int pitch) {
  noteOn(pitch, 0);
}


