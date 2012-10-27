#pragma once
#include "config.h"

//Compatibility with Proteus schematic
#define SHARP1 PA1
#define SHARP2 PA0
#define SHARP3 0x02
#define SHARP4 0x03
#define SLUM1 0x04
#define SLUM2 0x05

#define LED1 PC3
#define LED2 PC2
#define LED3 PC1
#define LED4 PC0

#define TEREN_BIT PD3
#define LED_TEREN_BIT PD4
#define DUMMY_IN1 PC6
#define M1IN1 PB0
#define ROLAIN1 PB1
#define M2IN1 PC7

#define D2PWM1 OCR2B
#define D2PWM2 OCR1A
#define D1PWM1 OCR0A
#define D1PWM2 OCR0B

#define M1_PORT PORTB
#define M2_PORT PORTC

//Sharp mapping
#define SHARP_STANGA SHARP1
#define SHARP_DREAPTA SHARP2
#define SHARP_JOS SHARP3
#define SHARP_SUS SHARP4

//LED control
#define set_led(led_bit) PORTC |= _BV(led_bit)
#define clear_led(led_bit) PORTC &= ~_BV(led_bit)
#define set_led_teren() PORTD |= _BV(LED_TEREN_BIT)
#define clear_led_teren() PORTD &= ~_BV(LED_TEREN_BIT)

//Rola Control
#define ROLA_POWER D1PWM2
#define ROLA_DIR_BIT ROLAIN1
#define SUCK_POWER 240
#define SPIT_POWER 250
#define HOLD_POWER 180
#define rola_suck() PORTB &= ~_BV(ROLA_DIR_BIT); ROLA_POWER = SUCK_POWER
#define rola_spit() PORTB |= _BV(ROLA_DIR_BIT); ROLA_POWER = SPIT_POWER
#define rola_fleghm() PORTB ^= _BV(ROLA_DIR_BIT)
#define rola_hold() PORTB &= ~_BV(ROLA_DIR_BIT); ROLA_POWER = HOLD_POWER

//Servo control
#define SERVO_POS D2PWM2
