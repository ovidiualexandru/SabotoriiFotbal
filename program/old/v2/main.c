#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "adc_blocking.h"
#include "leds.h"
#include "digsensors.h"

__attribute__((noreturn))
int main()
{
	initADC();
	initleds();
	initsensors();
	
	set_led(LED1);
	
	uint8_t val = readADC(SLUM1);
	
	read_dig(SHARP_STANGA);
	read_white();
	read_switch();
	for(;;);
}