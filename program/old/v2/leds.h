#pragma once
#include <avr/io.h>

//LED control
#define LED_PORT PORTC
#define LED_DDR DDRC
#define LED1 PC3
#define LED2 PC2
#define LED3 PC1
#define LED4 PC0

#define LED_TEREN_DDR DDRD
#define LED_TEREN_PORT PORTD
#define LED_TEREN_BIT PD4

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