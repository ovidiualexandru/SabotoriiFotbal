#include <avr/io.h>
/*******************************
ADC Blocking Library.

Reads the ADC in a blocking
fashion.
*******************************/

/*******************************
ADC Conversion
*******************************/
#define SHARP3 0x02
#define SHARP4 0x03
#define SLUM1 0x04
#define SLUM2 0x05

#define SHARP_JOS SHARP3
#define SHARP_SUS SHARP4

/******************************/

/*
 * Initialize the ADC reading
 */
void initADC();

/*
 * Blocking read an ADC channel specified by channel
 */
uint8_t readADC(uint8_t channel);