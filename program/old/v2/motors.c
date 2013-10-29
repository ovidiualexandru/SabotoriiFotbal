#include "rola.h"

void inittimers()
{
	//init GPIO drivere
	DDRC |= _BV(PC6) | _BV(PC7);
	DDRB |= _BV(PB0) | _BV(PB1) | _BV(PB3) | _BV(PB4);
	DDRD |= _BV(PD6) /* | _BV(PD7) */;
	//init timere motoare
	//Clear all settings, Paranoid Parrot © style
	ICR1 = 0;
	OCR1A = 0;
	OCR1B = 0;
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
	OCR0A = 0;
	OCR0B = 0;
	TCCR0A = 0;
	TCCR0B = 0;
	TIMSK0 = 0;
	OCR2A = 0;
	OCR2B = 0;
	TCCR2A = 0;
	TCCR2B = 0;
	TIMSK2 = 0;
	
	//Timer 0: control for first motor
	TCCR0B |= /*_BV(CS02) | */ _BV(CS01) /*| _BV(CS00)*/; //Prescaler 8
	TCCR0A |=  /*_BV(WGM01) |*/ _BV(WGM00); // Select mode, Phase correct
	TCCR0A |= _BV(COM0A1) /*| _BV(COM0A0)*/ | _BV(COM0B1) /*| _BV(COM0B0)*/; //Select COM mode, non-inverting on OC0A and OC0B for phase correct pwm ?
	
	//Timer 2: control for second motor
	TCCR2B |= /*_BV(CS22) | */ _BV(CS21) /*| _BV(CS20)*/; //Prescaler 8
	TCCR2A |= /*_BV(WGM21) |*/ _BV(WGM20); // Select Mode, Phase correct
	TCCR2A |= _BV(COM2A1) /*| _BV(COM2A0)*/ | _BV(COM2B1) /*| _BV(COM2B0)*/; //Select COM mode, non-inverting on OC2A  and OC0B for phase correct pwm ?
	
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


#define MOTOR_LEFT_POWER D1PWM1
#define MOTOR_RIGHT_POWER D2PWM1
#define motor_left_backward() M1_PORT &= ~_BV(M1IN1)
#define motor_left_forward() M1_PORT |= _BV(M1IN1)
#define motor_right_forward() M2_PORT |= _BV(M2IN1)
#define motor_right_backward() M2_PORT &= ~_BV(M2IN1)

#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 0

extern IO_Data io;

static inline void set_motor_left(uint8_t value, uint8_t dir)
{
	if( dir){
		motor_left_forward();
	}
	else{
		motor_left_backward();
	}
	MOTOR_LEFT_POWER = value;
}

static inline void set_motor_right(uint8_t value, uint8_t dir)
{
	if( dir){
		motor_right_forward();
	}
	else{
		motor_right_backward();
	}
	MOTOR_RIGHT_POWER = value;
}

#define STOP_TIME io.stop_time
static inline void stop()
{
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	wait_nbk(STOP_TIME);
}