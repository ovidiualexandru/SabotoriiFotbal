#include <avr/io.h>

//Compatibility with Proteus schematic
#define SHARP1 PA1
#define SHARP2 PA0
//Sharp mapping

#define SHARP_DDR DDRA
#define SHARP_PORT PORTA
#define SHARP_PIN PINA
#define SHARP_STANGA SHARP1
#define SHARP_DREAPTA SHARP2

#define TEREN_DDR DDRD
#define TEREN_PORT PORTD
#define TEREN_PIN PIND
#define TEREN_BIT PD3

#define SWITCH_DDR DDRA
#define SWITCH_PORT PORTA
#define SWITCH_PIN PINA
#define SWITCH1 PA6
#define SWITCH2 PA7

void initsensors();
uint8_t read_dig(uint8_t sharp);
uint8_t read_white();
uint8_t read_switch();