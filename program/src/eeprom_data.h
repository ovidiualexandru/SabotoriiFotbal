#pragma once
#include <avr/eeprom.h>
#include <stdint.h>
#include "config.h"


#define EEPROM_SIGNATURE 0xB00B
IO_Data EEMEM eep_io;
uint16_t EEMEM signature;

/*
 * Write configuration to the EEPROM
 */
void store_config( IO_Data* config)
{
	//write signature
	eeprom_busy_wait(); 	
	eeprom_update_word(&signature, EEPROM_SIGNATURE);
	//write configuration
	eeprom_busy_wait();
	eeprom_update_block( (void*) config, (void*) &eep_io, sizeof(IO_Data));
}

/*
 * Load a configuration, if there is none load a default
 */
void load_config(IO_Data* config)
{
	eeprom_busy_wait();
	uint16_t sig = eeprom_read_word(&signature);
	if( sig != EEPROM_SIGNATURE){ //signature not ok
		//create default config
		IO_Data default_config = { 
					.stop_time = 100,
					.get_ball_power = 240,
					.get_ball_time = 100, 
					.turn90_power = 180, 
					.turn90_time = 40,
					.turn180_power = 180,
					.turn180_time = 80,
					.go_back_power = 100,
					.go_back_time = 40,
					.veer_power_min = 130,
					.veer_power_max = 170,
					.go_forward_power = 180,
					.turn10_power = 100,
					.turn10_time = 10,
					.suck_power = 240,
					.spit_power = 250,
					.hold_power = 180,
					.servo_raise = 200,
					.servo_lower = 0,
					.servo_move_time = 100};
		store_config( &default_config);
	}
	eeprom_busy_wait();
	eeprom_read_block((void*) config, (void*)&eep_io, sizeof(IO_Data));
}