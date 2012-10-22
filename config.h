#pragma once

#include <stdint.h>
#include <util/delay.h>

typedef struct {
	//Motor config
	uint8_t stop_time;
	uint8_t get_ball_power;
	uint8_t get_ball_time;
	
	uint8_t turn90_power;
	uint8_t turn90_time;
	
	uint8_t turn180_power;
	uint8_t turn180_time;
	
	uint8_t go_back_power;
	uint8_t go_back_time;
	
	uint8_t veer_power_min;
	uint8_t veer_power_max;
	
	uint8_t go_forward_power;
	//Rola config
	uint8_t suck_power;
	uint8_t spit_power;
	uint8_t hold_power;
	
	//Servo config
	uint8_t servo_raise;
	uint8_t servo_lower;
	uint8_t servo_move_time;
	//Sensors - this should not be in EEPROM but the hell with it
} IO_Data;

//ms_x_10 is how many slices of 10 ms to wait
// should be < 255
static inline void wait_nbk(uint8_t ms_x_10)
{
	uint8_t i;
	for( i = 0; i < ms_x_10; ++i){
		_delay_ms(10);
	}
}

