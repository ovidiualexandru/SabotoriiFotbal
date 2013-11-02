#include <avr/io.h>
#include <stdint.h>
#include "sensors.h"

void initadc()
{
	//init ADC
	ADMUX |=  _BV(ADLAR);
	ADCSRA |= _BV(ADEN);
	ADCSRA |=  _BV(ADPS1) | _BV(ADPS2) | _BV(ADPS0);
}

uint8_t ana_sharp(uint8_t sensor)
{
	uint8_t ret;
	uint8_t admux = ADMUX;
	admux &= ~(_BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0));
	admux |= sensor;
	ADMUX = admux;
	ADCSRA |= _BV(ADSC);
	loop_until_bit_is_set(ADCSRA, ADIF);
	ret = ADCH;
	ADCSRA |= _BV(ADIF);
	return ret;
}