#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 20000000
#include <util/delay.h>
//defines to be used as param for read_adc
#define concat(a,b) a##b

#define SHARP1 PA1
#define SHARP2 PA0
#define SHARP3 0x02
#define SHARP4 0x03
#define SLUM1 0x04
#define SLUM2 0x05

#define SHARP_STANGA SHARP1
#define SHARP_DREAPTA SHARP2
#define SHARP_JOS SHARP3
#define SHARP_SUS SHARP4

#define read_dig(sharp) PINA & _BV(sharp)

//Compatibility with Proteus schematic
#define LED1 PC3
#define LED2 PC2
#define LED3 PC1
#define LED4 PC0

#define TEREN_BIT PD3
#define LED_TEREN_BIT PD4
#define DUMMY_IN1 PC6
#define M1IN1 PB0
#define ROLAIN1 PB1
#define M2IN1 PC7

#define D2PWM1 OCR2B
#define D2PWM2 OCR2A
#define D1PWM1 OCR0A
#define D1PWM2 OCR0B

#define M1_PORT PORTB
#define M2_PORT PORTC

#define set_led(led_bit) PORTC |= _BV(led_bit)
#define clear_led(led_bit) PORTC &= ~_BV(led_bit)
#define set_led_teren() PORTD |= _BV(LED_TEREN_BIT)
#define clear_led_teren() PORTD &= ~_BV(LED_TEREN_BIT)

#define rola_suck() PORTB &= ~_BV(PB1); ROLA_POWER = GET_BALL_POWER
#define rola_spit() PORTB |= _BV(PB1); ROLA_POWER = SHOOT_BALL_POWER
#define rola_fleghm() PORTB ^= _BV(PB1)
#define rola_hold() PORTB &= ~_BV(PB1); ROLA_POWER = HOLD_BALL_POWER


#define SERVO_POS D2PWM2
#define ROLA_POWER D1PWM2
#define MOTOR_LEFT_POWER D1PWM1
#define MOTOR_RIGHT_POWER D2PWM1
#define motor_left_backward() M1_PORT &= ~_BV(M1IN1)
#define motor_left_forward() M1_PORT |= _BV(M1IN1)
#define motor_right_forward() M2_PORT |= _BV(M2IN1)
#define motor_right_backward() M2_PORT &= ~_BV(M2IN1)

#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 0


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
	TIMSK1 |= _BV(TOIE1); // enable interrupt
	
	//init ADC
	ADMUX |=  _BV(ADLAR);
	ADCSRA |= _BV(ADEN);
	ADCSRA |=  _BV(ADPS1) | _BV(ADPS2) | _BV(ADPS0);
}

ISR(TIMER1_OVF_vect)
{
	rola_fleghm();
}

#define SHOOT_BALL_POWER 250
#define HOLD_BALL_POWER 180
#define GET_BALL_POWER 240
#define GET_BALL_DELAY 1000
inline static void get_ball()
{
	set_motor_left(GET_BALL_POWER, MOTOR_FORWARD);
	set_motor_right(GET_BALL_POWER, MOTOR_FORWARD);
	rola_suck();
	_delay_ms(GET_BALL_DELAY);
	rola_hold();
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
}

#define TURN90_POWER 180
#define TURN90_TIME 400
inline static void turn_left()
{
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(TURN90_TIME);
	set_motor_left(TURN90_POWER, MOTOR_BACKWARD);
	set_motor_right(TURN90_POWER, MOTOR_FORWARD);
	_delay_ms(TURN90_TIME);
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(TURN90_TIME);
}

inline static void turn_right()
{
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(TURN90_TIME);
	set_motor_left(TURN90_POWER, MOTOR_FORWARD);
	set_motor_right(TURN90_POWER, MOTOR_BACKWARD);
	_delay_ms(TURN90_TIME);
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(TURN90_TIME);
}

#define TURN180_POWER 180
#define TURN180_TIME 800
inline static void turn_around()
{
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(TURN180_TIME);	
	set_motor_left(TURN180_POWER, MOTOR_FORWARD);
	set_motor_right(TURN180_POWER, MOTOR_BACKWARD);
	_delay_ms(TURN180_TIME);
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(TURN180_TIME);
}


#define GO_BACK_TIME 400
#define GO_BACK_POWER 180
inline static void go_back()
{
	set_motor_left(GO_BACK_POWER, MOTOR_BACKWARD);
	set_motor_right(GO_BACK_POWER, MOTOR_BACKWARD);
	_delay_ms(GO_BACK_TIME);
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(GO_BACK_TIME);
}

#define VEER_POWER_MIN 130
#define VEER_POWER_MAX 170
inline static void veer_left()
{
	set_motor_left(VEER_POWER_MIN, MOTOR_FORWARD);
	set_motor_right(VEER_POWER_MAX, MOTOR_FORWARD);
}

inline static void veer_right()
{
	set_motor_left(VEER_POWER_MAX, MOTOR_FORWARD);
	set_motor_right(VEER_POWER_MIN, MOTOR_FORWARD);
}

#define GO_FORWARD_POWER 180
inline static void go_forward_nbk()
{
	set_motor_left(GO_FORWARD_POWER, MOTOR_FORWARD);
	set_motor_right(GO_FORWARD_POWER, MOTOR_FORWARD);
}

__attribute__((noreturn))
int main(void)
{
	init();
	sei();
	ROLA_POWER = 0;
	SERVO_POS = 0;
	_delay_ms(5000);
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
	veer_left();
	for(;;){
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
			veer_right();
		}
		if(sharp2){
			go_back();
			turn_left();
			veer_left();
		}
		if(/*sharp3 > 30 &&*/ sharp4 > 100){
			go_back();
			turn_around();
			veer_left();
		}
	}
}