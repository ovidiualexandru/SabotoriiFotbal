#include <avr/io.h>
#include <util/delay.h>
#include "defines.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"
#include "serial_comm.h"
#include "config.h"

threshold_t sensor_thresholds;

__attribute__((noreturn))
int main()
{
	initleds();
	initadc();
	initmotors();
	USART0_Init();

	uint8_t loaded_ok = load_config(&sensor_thresholds);
	if(!loaded_ok){
		set_led_teren();
		sensor_thresholds.jos = 100;
		sensor_thresholds.sus = 100;
		sensor_thresholds.lumina_stanga = 75;
		sensor_thresholds.lumina_dreapta = 75;
		store_config(&sensor_thresholds);
	}
	else{
		set_led(LED1);
		set_led(LED4);
	}
	
	for(;;);
}
