#include <avr/io.h>     // For I/O and other AVR registers
#include <util/delay.h> // For timing

int main(void) {

  for(;;) {
    _delay_ms(500);
    PORTB &= B11011111; 
    _delay_ms(500);
    PORTB |= B00100000; 
  }
  
  /* Compiler fix */
  return 0;
}
