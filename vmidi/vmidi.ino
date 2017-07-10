#define USB_RX_USER_HOOK(data, len)     rx_callback(data, len);

#include "UsbMIDI.h"

int led=0;
void rx_callback(uchar *data, uchar len){
  led = !led;
  digitalWrite(13,led);
}

void setup() {
  pinMode(13,OUTPUT);
  UsbMIDI.setup();
}
void loop() {

  UsbMIDI.update();
  /*UsbMIDI.noteOn(i,127);
  delay(300);
  UsbMIDI.noteOff(i);
  delay(300);
  */
}
