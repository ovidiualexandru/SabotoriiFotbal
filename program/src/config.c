#include "config.h"
#include <avr/eeprom.h>

#define EEPROM_SIGNATURE 0xCACA
threshold_t EEMEM eeprom_config;
uint16_t EEMEM signature;

void store_config(threshold_t* config)
{
	eeprom_busy_wait(); 	
	eeprom_update_word(&signature, EEPROM_SIGNATURE);
	eeprom_busy_wait();
	eeprom_update_block( (void*) config, (void*) &eeprom_config, sizeof(threshold_t));
}

uint8_t load_config(threshold_t* config)
{
	uint8_t succes = 0;
	eeprom_busy_wait();
	uint16_t sig = eeprom_read_word(&signature);
	if( sig == EEPROM_SIGNATURE){ //signature ok
		succes = 1;
		eeprom_busy_wait();
		eeprom_read_block((void*) config, (void*)&eeprom_config, sizeof(threshold_t));
	}
	return succes;
}