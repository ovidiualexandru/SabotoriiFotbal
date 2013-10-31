#pragma once
#include <avr/io.h>
#include "defines.h"

static inline void set_led(uint8_t led_bit)
{
	LED_PORT |= _BV(led_bit);
}

static inline void clear_led(uint8_t led_bit)
{
	LED_PORT &= ~_BV(led_bit);
}

static inline void set_led_teren()
{
	LED_TEREN_PORT |= _BV(LED_TEREN_BIT);
}

static inline void clear_led_teren()
{
	LED_TEREN_PORT &= ~_BV(LED_TEREN_BIT);
}

void initleds();