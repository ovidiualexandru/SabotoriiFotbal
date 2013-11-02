#pragma once

#include <avr/io.h>
#include <stdint.h>
#include "defines.h"

/*****************************************
LED Control
leds.h

LED control
*****************************************/

/*
 * Light up a LED. Valid arguments: LED1, LED2, LED3, LED4
 */
static inline void set_led(uint8_t led_bit)
{
	LED_PORT |= _BV(led_bit);
}

/*
 * Close a LED. Valid arguments: LED1, LED2, LED3, LED4
 */
static inline void clear_led(uint8_t led_bit)
{
	LED_PORT &= ~_BV(led_bit);
}

/*
 * Light up the field LED.
 */
static inline void set_led_teren()
{
	LED_TEREN_PORT |= _BV(LED_TEREN_BIT);
}

/*
 * Close the field LED.
 */
static inline void clear_led_teren()
{
	LED_TEREN_PORT &= ~_BV(LED_TEREN_BIT);
}

/*
 * Initialize the LEDs
 */
void initleds();