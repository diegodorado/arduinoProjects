#include <SoftwareSerial.h>

/*

Connect HC06 to pin 2 and 3
To change name type AT+NAMEDESIRED NAME, notice that there should be no space between the command and name. The module will reply OKyour set name

*/

SoftwareSerial mySerial (2,3); // RX, TX

void setup () {
    mySerial.begin(9600);
    Serial.begin(9600);
    Serial.println("Enter AT commands:");
}

void loop() {
    while (mySerial.available()) {
        Serial.write(mySerial.read());
    }

    while (Serial.available()) {
        mySerial.write(Serial.read());
    }
}
