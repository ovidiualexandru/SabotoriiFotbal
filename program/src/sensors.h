#pragma once

#include <avr/io.h>
#include <stdint.h>
#include "defines.h"

/*****************************************
Digital and analog sensors
sensors.h
Uses: ADC

Reads digital and analog sensors.
*****************************************/
/*
 * Read a digital sharp sensor. Valid arguments: DIG_SHARP_STANGA,
 * DIG_SHARP_RIGHT.
 */
#define dig_sharp(sharp) ((DIG_SHARP_PIN & _BV(sharp)) ? 0 : 1)

/*
 * Read the white sensor.
 */
#define dig_field() ( (FIELD_PIN & _BV(FIELD_BIT)) ? 0 : 1)
/****************************************/

/*
 * Read the stop pin - used for programming. Pull-up set in initleds().
 */
#define dig_stop() ( (STOP_PIN & _BV(STOP_BIT)) ? 0 : 1)

/*
 * Initialize the ADC.
 */
void initadc();
/*
 * Set a channel for analog reading.Valid arguments: ANA_SHARP_JOS, ANA_SHARP_SUS,
 * ANA_FOTO_LEFT, ANA_FOTO_RIGHT.
 */
void ana_set(uint8_t sensor);

/*
 * Read the currently selected analog channel
 */
uint8_t ana_read();
