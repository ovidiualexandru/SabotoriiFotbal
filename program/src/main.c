#include <avr/io.h>
#include <util/delay.h>
#include "defines.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"

__attribute__((noreturn))
int main()
{
	initleds();
	initadc();
	initmotors();
	
	set_led(LED1);
	_delay_ms(1000);
	
	clear_led(LED1);
	set_led(LED2);
	_delay_ms(1000);
	
	clear_led(LED2);
	set_led(LED3);
	_delay_ms(1000);
	
	clear_led(LED3);
	set_led(LED4);
	_delay_ms(1000);
	
	clear_led(LED4);
	_delay_ms(1000);
	
	for(;;){
		set_led_teren();
		uint8_t sus = ana_sharp(ANA_SHARP_SUS);
		uint8_t jos = ana_sharp(ANA_SHARP_JOS);
		uint8_t stanga = ana_sharp(ANA_FOTO_STANGA);
		uint8_t dreapta = ana_sharp(ANA_FOTO_DREAPTA);
		_delay_ms(500);
		clear_led_teren();
		
		clear_led(LED1);
		clear_led(LED2);
		clear_led(LED3);
		clear_led(LED4);
		
		if( sus > 100)	set_led(LED1);
		if( jos > 100) set_led(LED2);
		if( stanga > 150) set_led(LED3);
		if( dreapta > 150) set_led(LED4);
		_delay_ms(1000);
	}
}