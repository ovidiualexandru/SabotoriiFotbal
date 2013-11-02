#pragma once
/*****************************************
I/O Pins mapping.

Defines memory-mapped I/O to corresponding
signals.
*****************************************/

/*****************************************
Compatibility with Proteus schematic
*****************************************/
//Digital Sharp
#define SHARP1 PA1
#define SHARP2 PA0
//Analog Sharp
#define SHARP3 0x02
#define SHARP4 0x03
//Fototransistors
#define SLUM1 0x04
#define SLUM2 0x05

//LEDs
#define LED1 PC3
#define LED2 PC2
#define LED3 PC1
#define LED4 PC0
#define LED_TEREN_BIT PD4

//Reflectance sensor
#define TEREN_BIT PD3
//Unused
#define DUMMY_IN1 PC6

//Motor direction pins
#define M1IN1 PB0
#define M2IN1 PC7
#define ROLAIN1 PB1

//PWM pins, redefined below
#define D2PWM1 OCR2B
#define D2PWM2 OCR1A
#define D1PWM1 OCR0A
#define D1PWM2 OCR0B
/****************************************/

/*****************************************
Mapping for motors
*****************************************/
//Left motor control
#define D2PWM1_BIT PD6
#define D2PWM2_BIT PD5
#define D2PWM_PORT PORTD
#define D2PWM_DDR DDRD

//Right motor control
#define D1PWM1_BIT PB3
#define D1PWM2_BIT PB4
#define D1PWM_PORT PORTB
#define D1PWM_DDR DDRB

#define M1_DDR DDRB
#define M2_DDR DDRC
#define M1_PORT PORTB
#define M2_PORT PORTC

//Servo control
#define SERVO_POS D2PWM2
#define SERVO_POS_BIT D2PWM2_BIT
#define SERVO_POS_PORT D2PWM_PORT

//Roll control
#define ROLA_POWER D1PWM2
#define ROLA_DIR_PORT PORTB
#define ROLA_DIR_BIT ROLAIN1
#define ROLA_DIR_DDR  DDRB
/****************************************/

/*****************************************
Mapping for Sharp sensors
*****************************************/
//Digital Sharp
#define DIG_SHARP_STANGA SHARP1
#define DIG_SHARP_DREAPTA SHARP2
//Analog sharp
#define ANA_SHARP_SUS SHARP3
#define ANA_SHARP_JOS SHARP4
#define ANA_FOTO_STANGA SLUM1
#define ANA_FOTO_DREAPTA SLUM2

#define DIG_SHARP_PIN PINA
#define TEREN_PIN PIND
/****************************************/

/*****************************************
Mapping for LEDs
*****************************************/
#define LED_PORT PORTC
#define LED_DDR DDRC

#define LED_TEREN_DDR DDRD
#define LED_TEREN_PORT PORTD
/****************************************/
//Rola Control
#define SUCK_POWER 240
#define SPIT_POWER 250
#define HOLD_POWER 180
#define rola_suck() PORTB &= ~_BV(ROLA_DIR_BIT); ROLA_POWER = SUCK_POWER
#define rola_spit() PORTB |= _BV(ROLA_DIR_BIT); ROLA_POWER = SPIT_POWER
#define rola_stop() ROLA_POWER = 0
#define rola_fleghm() PORTB ^= _BV(ROLA_DIR_BIT)
#define rola_hold() PORTB &= ~_BV(ROLA_DIR_BIT); ROLA_POWER = HOLD_POWER
