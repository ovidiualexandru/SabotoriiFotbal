#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "defines.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"
#include "serial_comm.h"
#include "config.h"

threshold_t sensor_thresholds;


ISR(USART0_RX_vect)
{
	uint8_t command = UDR0;
	if(command == '$'){ // Read Sensors
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
		
		uint8_t threshold_jos = ( jos > sensor_thresholds.jos) ? 1 : 0;
		uint8_t threshold_sus = ( sus > sensor_thresholds.sus) ? 1 : 0;
		uint8_t threshold_stg = (lum_stanga > sensor_thresholds.lumina_stanga) \
			? 1 : 0;
		uint8_t threshold_drp = (lum_dreapta > sensor_thresholds.lumina_dreapta) \
			? 1 : 0;
		
		uint8_t first_byte = (stanga<<7) | (dreapta<<6) | (teren<<5) \
			| (threshold_jos<<4) | (threshold_sus<<3) | (threshold_stg<<2) \
			| (threshold_drp<<1);
		
		USART0_Transmit(first_byte);
		USART0_Transmit(jos);
		USART0_Transmit(sus);
		USART0_Transmit(lum_stanga);
		USART0_Transmit(lum_dreapta);

	}
	else if(command == '#'){ // Write thresholds
		uint8_t threshold_jos = USART0_Receive();
		uint8_t threshold_sus = USART0_Receive();
		uint8_t threshold_left = USART0_Receive();
		uint8_t threshold_right = USART0_Receive();
		sensor_thresholds.jos = threshold_jos;
		sensor_thresholds.sus = threshold_sus;
		sensor_thresholds.lumina_stanga = threshold_left;
		sensor_thresholds.lumina_dreapta = threshold_right;
		store_config(&sensor_thresholds);
	}
	else if(command == '%'){ //Read thresholds
		USART0_Transmit(sensor_thresholds.jos);
		USART0_Transmit(sensor_thresholds.sus);
		USART0_Transmit(sensor_thresholds.lumina_stanga);
		USART0_Transmit(sensor_thresholds.lumina_dreapta);
	}
}

__attribute__((noreturn))
int main()
{
	initleds();
	initadc();
	initmotors();
	set_servo(SERVO_MIN);
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
	uint8_t loaded_ok = load_config(&sensor_thresholds);
	if(!loaded_ok){
		set_led_teren();
		sensor_thresholds.jos = 100;
		sensor_thresholds.sus = 100;
		sensor_thresholds.lumina_stanga = 75;
		sensor_thresholds.lumina_dreapta = 75;
		store_config(&sensor_thresholds);
	}
	_delay_ms(1000);
	
	clear_led(LED4);
	clear_led_teren();
	_delay_ms(1000);
	set_servo(SERVO_MAX-2);
	USART0_Receive_sei();
	sei();
	for(;;){
		if( dig_teren()){
			clear_led_teren();
		}
		else set_led_teren();
	}
}
