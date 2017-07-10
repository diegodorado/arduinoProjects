/*


                                             -hmNh:     
                                              hmyoh     
                                              :h-:s:    
                                               s:-:y-   
                                               .s.:/oo. 
                                               -s.-:::s-
                                               o:..+---y
                                               y..-/---h
                                              -o../----h
                                             -s-.::--:/s
                                           `/+-.-:---:h.
                                         ./+-..::----s/ 
                                      .:/:.`..:-----s/  
                          ``..-----:::-.```.------/s:   
                     sy+///::-..```````..-------:oo.    
                     om/::-------::::::------:/so-      
                      `:+o/::::::/::::::::/oo+:`        
                          ./++ooooossoo++/-`            





Resistor Choice

Here are some guidelines for resistors but be sure to experiment for a desired response.

Use a 1 megohm resistor (or less maybe) for absolute touch to activate.
With a 10 megohm resistor the sensor will start to respond 4-6 inches away.
With a 40 megohm resistor the sensor will start to respond 12-24 inches away (dependent on the foil size). Common resistor sizes usually end at 10 megohm so you may have to solder four 10 megohm resistors end to end.
One tradeoff with larger resistors is that the sensor's increased sensitivity means that it is slower. Also if the sensor is exposed metal, it is possible that the send pin will never be able to force a change in the receive (sensor) pin, and the sensor will timeout.
Also experiment with small capacitors (100 pF - .01 uF) to ground, on the sense pin. They improve stability of the sensor.

*/

#include <CapacitiveSensor.h>

CapacitiveSensor cs[] = {
  CapacitiveSensor(7,2),
  CapacitiveSensor(7,3),
  CapacitiveSensor(7,4),
  CapacitiveSensor(7,5),
  CapacitiveSensor(7,6),
};

const int UMBRAL = 1000;
int estadoPines[] = {0,0,0,0,0};
int antirebotes[] = {0,0,0,0,0};
long maximos[] = {0,0,0,0,0};

int notas[] = {60,62,65,67,69};
int lastValue;

void setup()                    
{
  Serial.begin(9600);
}

void cambiarEstadoPin(int pin, int nuevoEstado) {
  estadoPines[pin] = nuevoEstado;
  antirebotes[pin] = 0;
  if(estadoPines[pin] == 1){
    noteOn(notas[pin], 100);
  }else{
    noteOff(notas[pin]);
  }
 
} 


void loop()                    
{
  for(int i=0;i<5;i++){

    long total =  cs[i].capacitiveSensor(15);
    if(total > maximos[i])
      maximos[i] = total;
      
    int umbral = maximos[i]/10;
    byte nuevoEstadoPin = (total > umbral);
    
    if (estadoPines[i] != nuevoEstadoPin ){
      antirebotes[i] = antirebotes[i] + 1;
  
      if(antirebotes[i] > 2 ){
        cambiarEstadoPin(i,nuevoEstadoPin);
/*
    Serial.print(i);
    Serial.print(":");
    Serial.print(total);
    Serial.println(" ;");
  */      
      }
    }else{
        antirebotes[i]= 0;
    }
  }
}




//  plays a MIDI note.
void noteOn(int pitch, int velocity) {
  //return;
  Serial.write(0x90);
  Serial.write(pitch);
  Serial.write(velocity);
}

void noteOff(int pitch) {
  noteOn(pitch, 0);
}
