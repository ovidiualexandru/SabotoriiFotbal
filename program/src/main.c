#include <avr/io.h>
#include <util/delay.h>
#include "defines.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"
#include "serial_comm.h"

__attribute__((noreturn))
int main()
{
	initleds();
	initadc();
	initmotors();
	USART0_Init();
	
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
		// uint8_t stanga = ana_sharp(ANA_FOTO_STANGA);
		// uint8_t dreapta = ana_sharp(ANA_FOTO_DREAPTA);
		uint8_t stanga = dig_sharp(DIG_SHARP_STANGA);
		uint8_t dreapta = dig_sharp(DIG_SHARP_DREAPTA);
		_delay_ms(500);
		clear_led_teren();
		
		clear_led(LED1);
		clear_led(LED2);
		clear_led(LED3);
		clear_led(LED4);
		
		if( sus > 100) set_led(LED1);
		if( jos > 100) set_led(LED2);
		if( stanga ) set_led(LED3);
		if( dreapta ) set_led(LED4);
		
		USART0_Transmit(sus);
		USART0_Transmit(jos);
		USART0_Transmit(stanga);
		USART0_Transmit(dreapta);
		
		set_led_teren();
		USART0_Receive();
		clear_led_teren();
		_delay_ms(1000);
	}
}
