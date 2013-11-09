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
		uint8_t command = USART0_Receive();
		if(command == '$'){
			ana_set(ANA_SHARP_JOS);
			ana_read();
			uint8_t jos = ana_read();
			
			ana_set(ANA_SHARP_SUS);
			ana_read();
			uint8_t sus = ana_read();
			
			ana_set(ANA_FOTO_STANGA);
			ana_read();
			uint8_t lum_stanga = ana_read();
			
			ana_set(ANA_FOTO_DREAPTA);
			ana_read();
			uint8_t lum_dreapta = ana_read();
			
			uint8_t stanga = dig_sharp(DIG_SHARP_STANGA);
			uint8_t dreapta = dig_sharp(DIG_SHARP_DREAPTA);
			uint8_t teren = dig_teren();
			
			uint8_t threshold_sus = (sus > 100) ? 1 : 0;
			uint8_t threshold_jos = (jos > 100) ? 1 : 0;
			uint8_t threshold_stg = (lum_stanga > 100) ? 1 : 0;
			uint8_t threshold_drp = (lum_dreapta > 100) ? 1 : 0;
			
			uint8_t first_byte = (stanga<<7) | (dreapta<<6) | (teren<<5) \
				| (threshold_sus<<4) | (threshold_jos<<3) | (threshold_stg<<2) \
				| (threshold_drp<<1);
			
			USART0_Transmit(first_byte);
			USART0_Transmit(jos);
			USART0_Transmit(sus);
			USART0_Transmit(lum_stanga);
			USART0_Transmit(lum_dreapta);
		}
		
	}
}
