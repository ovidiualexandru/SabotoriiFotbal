#include <avr/io.h>
#include "defines.h"

//Rola Control
#define ROLA_POWER D1PWM2
#define ROLA_DIR_BIT ROLAIN1
#define SUCK_POWER 240
#define SPIT_POWER 250
#define HOLD_POWER 180
#define rola_suck() PORTB &= ~_BV(ROLA_DIR_BIT); ROLA_POWER = SUCK_POWER
#define rola_spit() PORTB |= _BV(ROLA_DIR_BIT); ROLA_POWER = SPIT_POWER
#define rola_stop() ROLA_POWER = 0
#define rola_fleghm() PORTB ^= _BV(ROLA_DIR_BIT)
#define rola_hold() PORTB &= ~_BV(ROLA_DIR_BIT); ROLA_POWER = HOLD_POWER

void initrola();
void rola_in();
void rola_out();
void rola_hold();