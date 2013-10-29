#include "adc_blocking.h"

void initADC()
{
	ADMUX = 0;
	ADMUX |= /*_BV(REFS0) | _BV(REFS1) | */ _BV(ADLAR); // bit reference
	ADCSRA |= _BV(ADEN);
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

uint8_t readADC(uint8_t channel)
{
	uint8_t ret = 0;
	uint8_t admux = ADMUX;
	admux &= ~(_BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0));
	admux |= channel;
	ADMUX = admux;
	ADCSRA |= _BV(ADSC);
	loop_until_bit_is_set(ADCSRA, ADIF);
	ret = ADCH;
	ADCSRA |= _BV(ADIF);
	return ret;
}