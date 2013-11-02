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
 * Read a digital sharp sensor. Valid arguments: DIG_SHARP_STANGA, DIG_SHARP_STANGA
 */
#define dig_sharp(sharp) ((DIG_SHARP_PIN & _BV(sharp)) ? 1 : 0)

/*
 * Read the white sensor.
 */
#define dig_teren() ( (TEREN_PIN & _BV(TEREN_BIT)) ? 1 : 0)
/****************************************/

/*
 * Initialize the ADC.
 */
void initadc();

/*
 * Read an analog sharp sensor. Valid arguments: ANA_SHARP_JOS, ANA_SHARP_SUS,
 * ANA_FOTO_STANGA, ANA_FOTO_DREAPTA.
 */
uint8_t ana_sharp(uint8_t sensor);



