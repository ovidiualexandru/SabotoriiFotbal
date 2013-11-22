#pragma once

#include <stdint.h>

/*****************************************
Configuration loading and saving
config.h

Configurations that can be modified without
reprogramming. Stores values in EEPROM. Saved
values are threshold values for analog sensors.
*****************************************/

/*
 * Structure that holds threshold values
 */
struct thresholds_struct{
	uint8_t ambient_right;
	uint8_t ambient_left;

} __attribute__((__packed__));

typedef struct thresholds_struct threshold_t;
/****************************************/

/*
 * Load the configuration. If no configuration is present, returns 0, else 
 * returns 1.
 */
uint8_t load_config(threshold_t* config);

/*
 * Store the configuration.
 */
void store_config(threshold_t* config);
