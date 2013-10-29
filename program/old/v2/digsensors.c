#include "digsensors.h"

void initsensors()
{
	SWITCH_PORT |= _BV(SWITCH1) | _BV(SWITCH2);
}

uint8_t read_dig(uint8_t sharp)
{
	uint8_t ret = (SHARP_PIN & _BV(sharp)) ? 1 : 0;
	return ret;
}

uint8_t read_white()
{
	uint8_t ret = (TEREN_PIN & _BV(TEREN_BIT)) ? 1 : 0;
	return ret;
}

uint8_t read_switch()
{
	return SWITCH_PIN >> 6;
}