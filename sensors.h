#pragma once

#define read_dig(sharp) PINA & _BV(sharp)

static inline uint8_t read_adc(uint8_t sensor)
{
	uint8_t ret;
	uint8_t admux = ADMUX;
	admux &= 0xE0; //clear MUX4:0
	admux |= sensor;
	ADMUX = admux;
	ADCSRA |= _BV(ADSC);
	loop_until_bit_is_set(ADCSRA, ADIF);
	ret = ADCH;
	ADCSRA |= _BV(ADIF);
	return ret;
}

static inline uint8_t read_teren()
{
	uint8_t teren = PIND & _BV(PD3);
	return teren;
}