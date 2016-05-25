/*Port Definitions*/
int Max7219_pinCLK = 10;
int Max7219_pinCS = 9;
int Max7219_pinDIN = 8;

int vco_Pin = 3;


byte steps[8][8]={
  {255,0,0,0,255,0,0,100,},
  {140,110,0,0,100,0,20,50,},
  {100,0,100,0,255,0,100,0,},
  {255,0,100,0,100,0,100,0,},
  {255,0,100,0,255,0,100,0,},
  {255,0,100,0,255,0,100,0,},
  {255,0,100,0,255,0,100,0,},
  {255,0,100,0,255,0,100,0,}
};
 
byte scene = 0;
byte scenePart = 0;
byte stepIndex = 0;
int stepDuration = 250;
 
 
 
void Write_Max7219_byte(unsigned char DATA) 
{   
            byte i;
	    //digitalWrite(Max7219_pinCS,LOW);		
	    for(i=8;i>=1;i--)
          {		  
             digitalWrite(Max7219_pinCLK,LOW);
             digitalWrite(Max7219_pinDIN,DATA&0x80);// Extracting a bit data
             DATA = DATA<<1;
             digitalWrite(Max7219_pinCLK,HIGH);
           }                                 
}
 
 
void Write_Max7219(unsigned char address,unsigned char dat)
{
        digitalWrite(Max7219_pinCS,LOW);
        Write_Max7219_byte(address);           //address，code of LED
        Write_Max7219_byte(dat);               //data，figure on LED 
        digitalWrite(Max7219_pinCS,HIGH);
}
 
void Init_MAX7219(void)
{
 Write_Max7219(0x09, 0x00);       //decoding ：BCD
 Write_Max7219(0x0a, 0x03);       //brightness 
 Write_Max7219(0x0b, 0x07);       //scanlimit；8 LEDs
 Write_Max7219(0x0c, 0x01);       //power-down mode：0，normal mode：1
 Write_Max7219(0x0f, 0x00);       //test display：1；EOT，display：0
}
 
 
 
void setup()
{
 
  pinMode(Max7219_pinCLK,OUTPUT);
  pinMode(Max7219_pinCS,OUTPUT);
  pinMode(Max7219_pinDIN,OUTPUT);
  pinMode(vco_Pin, OUTPUT);   // sets the pin as output

  delay(50);
  Init_MAX7219();
  Serial.begin(9600);      // open the serial port at 9600 bps:    
}
 

 
 
void printLeds()
{ 
  int lines[6];
  
  for(byte i=0;i<6;i++){
    lines[i] = 0;
    for(byte j=0;j<8;j++)
    {
      if(steps[scene*2+scenePart][j] > i * 256 / 6 )
        lines[i] |= (0x80>>j);    
    }
  }
  
  
  for(byte i=1;i<=6;i++)
    Write_Max7219(i,lines[6-i]);
  
  
  Write_Max7219(7,(0x80>>scenePart));
  Write_Max7219(8,(0x80>>(stepIndex%8)));
}


void loop()
{ 
  analogWrite(vco_Pin, steps[scene*2+scenePart][stepIndex]);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255

  printLeds();
  delay(stepDuration);
  stepIndex++;
  stepIndex %=16;   
  scenePart = stepIndex/8;
}




