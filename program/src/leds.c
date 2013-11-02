#include "leds.h"
#include <avr/io.h>

void initleds()
{
	LED_DDR |= _BV(LED1) | _BV(LED2) | _BV(LED3) | _BV(LED4);
	LED_TEREN_DDR |= _BV(LED_TEREN_BIT);
}
