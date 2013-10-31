#include <avr/io.h>
#include <util/delay.h>
#include "defines.h"
#include "leds.h"

__attribute__((noreturn))
int main()
{
	initleds();
	for(;;){
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
		set_led_teren();
		_delay_ms(1000);
		clear_led_teren();
		_delay_ms(1000);
	}
}