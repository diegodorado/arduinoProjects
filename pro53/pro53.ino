#include <ClickEncoder.h>
#include <TimerOne.h>
#include <ClickButton.h>


const int controlsCount = 4;
const int statesCount = 12;

// the LED
const int ledPins[controlsCount] = { 7, 11, A1 , A5};
uint8_t state = 0;
uint8_t ledStates[statesCount][controlsCount];
uint8_t encodersValue[statesCount][controlsCount];


ClickEncoder mainEncoder = ClickEncoder(2, 3, 0, 4);

ClickEncoder encoders[controlsCount] = {
  ClickEncoder(4, 5),
  ClickEncoder(8, 9),
  ClickEncoder(12, 13),
  ClickEncoder(A2, A3),
};

ClickButton buttons[controlsCount] = {
  ClickButton (6, HIGH),
  ClickButton (10, HIGH),
  ClickButton (A0, HIGH),
  ClickButton (A4, LOW, HIGH), //button3 is active low
};

void timerIsr() {
  mainEncoder.service();
  for (int i=0; i<controlsCount; i++)
  {
    encoders[i].service();
  }   
}

void setup() {
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);   

  mainEncoder.setAccelerationEnabled(false);
  for (int i=0; i<statesCount; i++)
  {
    for (int j=0; i<controlsCount; i++)
    {
      ledStates[i][j] = 0;
      encodersValue[i][j] = 0;      
    }
  }
      
  for (int i=0; i<controlsCount; i++)
  {
    pinMode(ledPins[i],OUTPUT);
    buttons[i].debounceTime   = 20;   // Debounce timer in ms
  }  

  updateLeds();

  Serial.begin(9600);
  
  

        
}

void sendData(uint8_t controlId, uint8_t value)
{
  uint8_t msg[2];
  msg[0] = controlId;
  msg[1] = value;
  Serial.write(msg,2);
}    


void updateLeds()
{
  for (int i=0; i<controlsCount; i++)
  {
    if(i==3) //only led 3 is active high
      digitalWrite(ledPins[i],ledStates[state][i]);
    else
      digitalWrite(ledPins[i],!ledStates[state][i]);
  }  
}

void loop() {  
  int16_t value;

  value = mainEncoder.getValue();
  
  if (value != 0) 
  {
    state += statesCount + value;
    state %= statesCount;
    sendData(0, state);    
    updateLeds();
    
  }
  else
  {
    for (int i=0; i<controlsCount; i++)
    {
      
      value = encoders[i].getValue();
      
      if (value != 0) {
        encodersValue[state][i] += value;
        sendData(i + 1, encodersValue[state][i]);
      }
      
      // Update state of all buitton
      buttons[i].Update();
    
      if (buttons[i].clicks)
      {
        ledStates[state][i] = !ledStates[state][i];
        sendData( i + 1 + 4, ledStates[state][i]); //offset 4 for buttons
        updateLeds();
      }
    }
    
  }

  


      
}

