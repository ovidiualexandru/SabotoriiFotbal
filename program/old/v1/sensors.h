#pragma once

#define read_dig(sharp) ((PINA & _BV(sharp)) ? (1) : (0))

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

static inline uint8_t read_adc_filtered(uint8_t sensor)
{
	uint8_t values[10];
	uint8_t i, i_max = 0, i_min = 0, val_max = 0, val_min = 255;
	uint16_t sum = 0;
	uint8_t ret;
	//read 10 values, retain the max one and minimum one
	for(i = 0; i < 10; ++i){
		uint8_t val = read_adc(sensor);
		if(val < val_min){ val_min = val; i_min = i;}
		if(val > val_max){ val_max = val; i_max = i;}
		values[i] = val;
	}
	//add them up without the extreme values( min and max)
	for(i = 0; i < 10; ++i){
		uint8_t val = 0;
		if( i != i_min && i != i_max){
			val = values[i];
		}
		sum += val;
	}
	// divide by 8 to get the mean value
	ret = sum >> 3;
	return ret;
}

static inline uint8_t read_teren()
{
	uint8_t teren = PIND & _BV(PD3);
	return teren ? 1 : 0;
}