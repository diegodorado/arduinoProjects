

// These constants won't change:
const int sensorPin = A0;    // pin that the sensor is attached to
const int ledPin = 9;        // pin that the LED is attached to

// variables:
int sensorValue = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value
int noiseVal = 30; //dont read below this value


void setup() {
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.begin(9600); 
  
  // calibrate during the first five seconds 
  while (millis() < 10000) {
    sensorValue = analogRead(sensorPin);


    if (sensorValue > noiseVal) {
  
      // record the maximum sensor value
      if (sensorValue > sensorMax) {
        sensorMax = sensorValue;
      }
  
      // record the minimum sensor value
      if (sensorValue < sensorMin) {
        sensorMin = sensorValue;
      }
    
    
      Serial.print("min: "); 
      Serial.print( sensorMin ); 
      Serial.print( "    max: " ); 
      Serial.println(sensorMax); 
    }
     delay(1);
    
  }




  

  // signal the end of the calibration period
  digitalWrite(13, LOW);
}

void loop() {
  // read the sensor:
  sensorValue = analogRead(sensorPin);


  if (sensorValue > noiseVal) {
  
  
    // apply the calibration to the sensor reading
    sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);
  
    // in case the sensor value is outside the range seen during calibration
    sensorValue = constrain(sensorValue, 0, 255);
  
    Serial.println("val: " + sensorValue); 
  }
   delay(1);
}
