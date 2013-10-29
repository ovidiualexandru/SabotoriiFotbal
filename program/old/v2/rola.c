#include "rola.h"

void initrola()
{
	//init timer1
	ICR1 = 390;
	TCCR1A |= _BV(WGM11) /* | _BV(WGM10) */;
	TCCR1B |= _BV(WGM13) | _BV(WGM12); // Fast PWM, TOP ICR1, top ICR1
	TCCR1B |= _BV(CS12) /*| _BV(CS11)*/ | _BV(CS10); // prescaler 1024
	TCCR1A |= _BV(COM1A1); //output OC1A, non-inverting
}

void rola_in()
{

}

void rola_out()
{

}

void rola_hold()
{

}