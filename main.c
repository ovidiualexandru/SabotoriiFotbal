#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 20000000
#include <util/delay.h>
//defines to be used as param for read_adc
#include "defines.h"
#include "motor_control.h"
#include "sensors.h"
#include "config.h"
#include "eeprom_data.h"
#include "serial_comm.h"

IO_Data io;

static inline void init()
{
	//init GPIO leduri
	DDRC |= _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
	DDRD |= _BV(PD4);
	//init GPIO drivere
	DDRC |= _BV(PC6) | _BV(PC7);
	DDRB |= _BV(PB0) | _BV(PB1) | _BV(PB3) | _BV(PB4);
	DDRD |= _BV(PD6) | _BV(PD7);
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
	ICR1 = 0xFFFF;
	TCCR1B |= _BV(WGM13); // Phase and freq correct, top ICR1
	TCCR1B |= _BV(CS12) /*| _BV(CS11)*/ | _BV(CS10); // prescaler 1024
	// TIMSK1 |= _BV(TOIE1); // enable interrupt
	
	//init ADC
	ADMUX |=  _BV(ADLAR);
	ADCSRA |= _BV(ADEN);
	ADCSRA |=  _BV(ADPS1) | _BV(ADPS2) | _BV(ADPS0);
	
	load_config(&io);
	USART0_Init();
}

ISR(TIMER1_OVF_vect)
{
	rola_fleghm();
}

static inline void move_test()
{
	set_led_teren();
	turn_left();
	_delay_ms(500);
	turn_right();
	_delay_ms(500);
	turn_around();
	_delay_ms(500);
	turn_around();
	_delay_ms(500);
	clear_led_teren();
}

static inline void test_loop()
{	
	uint8_t sharp1, sharp2, sharp3, sharp4;
	sharp1 = !(read_dig(SHARP_STANGA));
	sharp2 = !(read_dig(SHARP_DREAPTA));
	sharp3 = read_adc(SHARP_JOS);
	sharp4 = read_adc(SHARP_SUS);
	if(sharp1)	set_led(LED1);
	else			clear_led(LED1);
	if(sharp2)	set_led(LED2);
	else			clear_led(LED2);
	if(sharp3 > 30)	set_led(LED3);
	else			clear_led(LED3);
	if(sharp4 > 100)	set_led(LED4);
	else			clear_led(LED4);
	/*
	if(sharp3 > 30 && sharp4 <= 30){
		get_ball();
		for(;;);
	}
	*/
	if( sharp1 ){
		go_back();
		turn_right();
		veer_right_nbk();
	}
	if(sharp2){
		go_back();
		turn_left();
		veer_left_nbk();
	}
	if(/*sharp3 > 30 &&*/ sharp4 > 100){
		go_back();
		turn_around();
		veer_left_nbk();
	}
}

__attribute__((noreturn))
int main(void)
{
	init();
	sei();
	ROLA_POWER = 0;
	SERVO_POS = 0;
	_delay_ms(5000);
	//test movement
	move_test();
	//start searching
	veer_left_nbk();
	//this loop indefinetely
	for(;;){
		test_loop();
	}
}