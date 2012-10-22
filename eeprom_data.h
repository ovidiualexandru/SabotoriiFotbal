#pragma once
#include <avr/eeprom.h>
#include <stdint.h>
#include "config.h"


#define EEPROM_SIGNATURE 0xBADE
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
		IO_Data default_config = { 100, 240, 100, 180, 40, 180, 80, 180, 40, 130, 170, 180, 240,	250,	180,	200,	0};
		store_config( &default_config);
	}
	eeprom_busy_wait();
	eeprom_read_block((void*) config, (void*)&eep_io, sizeof(IO_Data));
}