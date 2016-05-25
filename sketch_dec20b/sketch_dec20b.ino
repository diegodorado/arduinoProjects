#include <DueTimer.h>

struct SeqNote{
  byte note;
  byte velocity;
  uint32_t time;  
};

#define SEQ_COUNT 1000
#define MAX_SEQ_COUNT (UINT32_MAX-1) //in millis

byte command;
byte note;
byte velocity;
volatile uint32_t freq = 0;
SeqNote seq[SEQ_COUNT];
volatile uint32_t seq_index = 0;
volatile uint32_t seq_last_index = 0;
volatile uint32_t seq_counter = 0;
volatile uint32_t seq_length = 0;
volatile bool seq_on = false;
volatile bool prepared_to_rec = false;
volatile bool rec_on = false;


int myLed = 13;
bool ledOn = false;

void seqHandler(){
  if(rec_on){  
    if(seq_counter>=seq_length){
      Serial.println("rec overload 1");
      
      stopRec();
      startSeq();
    }
    seq_counter++;
  }


  if(seq_on){
    if(seq_counter>=seq_length){
      seq_counter=0;
      seq_index=0; 
      Serial.println("loop seq");
      return;         
    }
    
    if(seq_counter>=seq[seq_index].time){
      changeFreq(seq[seq_index].note);
      if(seq_index < seq_last_index)
        seq_index++;  
    }
    seq_counter++;    
  }
  
  
}

void changeFreq(int note){
  static int last_note = 0;

  if(note != last_note){
    last_note = note;
    Serial.println("change freq");
    int val = map(note, 48, 96, 0, 4095);
    analogWrite(DAC0, val);
    
  }
  
}


void setup() {

    
  analogWriteResolution(12);
  Serial.begin(9600);        
  Serial1.begin(31250);        
  analogWrite(DAC0, 0);
  Serial.println("started");

  pinMode(myLed, OUTPUT);
  Timer3.attachInterrupt(seqHandler);
  Timer3.start(1000); // Calls every 1ms

}


void loop () {
  
 if (Serial1.available()>2){

      int count = Serial1.available();//read first byte
      command = Serial1.read();//read first byte
      note = Serial1.read();//read next byte
      velocity = Serial1.read();//read final byte

    
   // note on message starting
   if (command== 144){ 
    Serial.print(command);
    Serial.print(" - ");
    Serial.print(note);
    Serial.print(" - ");
    Serial.print(velocity);
    Serial.print(" - ");
    Serial.println(count);
    
    if (note >=48 && note <=96){ 
       if(prepared_to_rec)
         startRec();

      if(rec_on){
        SeqNote seqn = { note , velocity , seq_counter };
        seq[seq_index] = seqn;
        seq_last_index = seq_index;
        seq_index++;

        if (seq_index>=SEQ_COUNT){
          Serial.println("rec overload 2");
          stopRec();
          startSeq();
        }
      }
        
      changeFreq(note);
     }
   }

   // note off message starting starting
   if (command== 128){ 
   }


  //transport
   if (command== 176){ 
      //rec button
     if (note== 117 && velocity>0){ 
        prepared_to_rec= true;
     }
      //play button
     if (note== 115 && velocity>0){ 
      if(prepared_to_rec)
        startRec();
       else
        startSeq();
     }
      //stop button
     if (note== 114 && velocity>0){ 
      if(seq_on){
        stopSeq();
      }
      if(rec_on){
        stopRec();
      }
     }
   }


   
 }


}


void startRec(){
  Serial.println("start recording");
  seq_length = MAX_SEQ_COUNT;
  rec_on = true;
  seq_counter =  0;
  seq_index = 0;
  prepared_to_rec= false;  
 
}

void stopRec(){  
  Serial.println("stop recording");
  seq_length = seq_counter;
  rec_on = false;
  seq_counter=0;    
  seq_index=0;
  prepared_to_rec= false;  
}

void startSeq(){
  Serial.println("start seq");
  seq_on = true;
  seq_counter=0;    
  seq_index=0;
}


void stopSeq(){
  Serial.println("stop seq");
  seq_on = false;
  prepared_to_rec= false;  
}

