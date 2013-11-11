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
#define LED_FIELD_BIT PD4

//STOP PIN
#define STOP_PIN PINC
#define STOP_PORT PORTC
#define STOP_BIT PC4

//Reflectance sensor
#define FIELD_BIT PD3
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
#define ROLLER_POWER D1PWM2
#define ROLLER_DIR_PORT PORTB
#define ROLLER_DIR_BIT ROLAIN1
#define ROLLER_DIR_DDR  DDRB
/****************************************/

/*****************************************
Mapping for Sharp sensors
*****************************************/
//Digital Sharp
#define DIG_SHARP_LEFT SHARP1
#define DIG_SHARP_RIGHT SHARP2
//Analog sharp
#define ANA_SHARP_UP SHARP3
#define ANA_SHARP_DOWN SHARP4
#define ANA_FOTO_LEFT SLUM1
#define ANA_FOTO_RIGHT SLUM2

#define DIG_SHARP_PIN PINA
#define FIELD_PIN PIND
/****************************************/

/*****************************************
Mapping for LEDs
*****************************************/
#define LED_PORT PORTC
#define LED_DDR DDRC

#define LED_FIELD_DDR DDRD
#define LED_FIELD_PORT PORTD
/****************************************/
