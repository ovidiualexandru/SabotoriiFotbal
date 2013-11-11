#include "leds.h"
#include <avr/io.h>

void initleds()
{
	LED_DDR |= _BV(LED1) | _BV(LED2) | _BV(LED3) | _BV(LED4);
	LED_FIELD_DDR |= _BV(LED_FIELD_BIT);
	STOP_PORT |= _BV(STOP_BIT);
}
