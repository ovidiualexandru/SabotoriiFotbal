#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "defines.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"
#include "serial_comm.h"
#include "config.h"

threshold_t sensor_thresholds;

#define BD_NOWALL_NOBALL 0
#define BD_NOWALL_MEDIUMBALL 0x01
#define BD_NOWALL_CLOSEBALL 0x02
#define BD_MEDIUMWALL_NOBALL 0x10
#define BD_MEDIUMWALL_MEDIUMBALL 0x11
#define BD_MEDIUMWALL_CLOSEBALL 0x12
#define BD_CLOSEWALL_NOBALL 0x20
#define BD_CLOSEWALL_CLOSEBALL 0x22
#define BD_TOOCLOSE 0xFF

#define BD_NOWALL 0x00
#define BD_MEDIUMWALL 0x10
#define BD_CLOSEWALL 0x20
#define BD_NOBALL 0x00
#define BD_MEDIUMBALL 0x01
#define BD_CLOSEBALL 0x02

uint8_t ball_detection(uint8_t up, uint8_t down)
{
	uint8_t ret = BD_NOWALL_NOBALL;
	uint8_t wall = BD_NOWALL;
	uint8_t ball = BD_NOBALL;
	if( up > 45 ) wall = BD_MEDIUMWALL;
	if( up > 95 ) wall = BD_CLOSEWALL;
	
	if( down > up && (down - up > 15)){
		if( wall == BD_NOWALL ) ball = BD_MEDIUMBALL;
		if( wall == BD_MEDIUMWALL ) ball = BD_CLOSEBALL;
		if( wall == BD_CLOSEWALL ) ball = BD_CLOSEBALL;
	}
	ret = wall | ball;
	if( wall == BD_CLOSEWALL && up > down) ret = BD_TOOCLOSE;
	return ret;
}

#define FOOTBALL_STATE_SEARCH 0
#define FOOTBALL_STATE_TOOCLOSE_RIGHT 1
#define FOOTBALL_STATE_TOOCLOSE_LEFT 2
#define FOOTBALL_STATE_SEE_BALL 3
#define FOOTBALL_STATE_CENTER_AND_SHOOT 4

void go_forward()
{
	set_motor_left(220, MOTOR_FORWARD);
	set_motor_right(220, MOTOR_FORWARD);
	_delay_ms(1000);
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(300);
}

void turn_around()
{
	set_motor_left(200, MOTOR_FORWARD);
	set_motor_right(200, MOTOR_BACKWARD);
	_delay_ms(600);
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(300);
}

void get_ball()
{
	set_servo(SERVO_MIN);
	_delay_ms(500);
	set_roller(255, MOTOR_FORWARD);
	go_forward();
	set_servo(SERVO_MAX - 2);
	set_roller(150, MOTOR_FORWARD);
	_delay_ms(500);
}


#define switch_state(STATE) state = STATE;\
	substate = 0; \
	i = 0; \
	set_motor_left(0, MOTOR_FORWARD); \
	set_motor_right(0, MOTOR_FORWARD); \
	_delay_ms(500);
	
void football_logic()
{
	static uint8_t state = FOOTBALL_STATE_SEARCH;
	static uint8_t substate = 0;
	static uint8_t i = 0;
	uint8_t j = 0;
	uint16_t acc_down = 0, acc_up = 0;
	switch(state){
		case FOOTBALL_STATE_SEARCH:
			//go forward a little
			//check digital sharps
				//if set, is next to wall, go somewhere else
			//check analog sharps
				//if can see ball, goto state SEE_BALL or just get it
			ana_set(ANA_SHARP_DOWN);
			ana_read();
			for( j = 0; j < 4; j++){
				acc_down = ana_read();
				}
			uint8_t down = acc_down >> 2;
		
			ana_set(ANA_SHARP_UP);
			ana_read();
			for( j = 0; j < 4; j++){
				acc_up = ana_read();
				}
			uint8_t up= acc_up >> 2;
			
			uint8_t ballpos = ball_detection(up, down);
			if(dig_sharp(DIG_SHARP_LEFT) || ballpos == BD_TOOCLOSE){
				switch_state(FOOTBALL_STATE_TOOCLOSE_LEFT);
				break;
			}
			if( dig_sharp(DIG_SHARP_RIGHT)){
				switch_state(FOOTBALL_STATE_TOOCLOSE_RIGHT);
				break;
			}
			if( (ballpos & 0x0F) != BD_NOBALL){
				switch_state(FOOTBALL_STATE_SEE_BALL);
				break;
			}
			switch(substate){
				case 0: //SCAN LEFT
					set_motor_left(120, MOTOR_BACKWARD);
					set_motor_right(120, MOTOR_FORWARD);
					_delay_ms(10);
					i++;
					if( i == 120){
						substate = 1;
						i = 0;
					}
					break;
				case 1: //SCAN RIGHT
					set_motor_left(120, MOTOR_FORWARD);
					set_motor_right(120, MOTOR_BACKWARD);
					_delay_ms(10);
					i++;
					if( i == 200){
						substate = 2;
						i = 0;
					}
					break;
				case 2: //STRAIGHTEN
					set_motor_left(120, MOTOR_BACKWARD);
					set_motor_right(120, MOTOR_FORWARD);
					_delay_ms(10);
					i++;
					if( i == 90){
						substate = 3;
						i = 0;
					}
					break;				
				case 3: //GO FORWARD
					set_motor_left(150, MOTOR_FORWARD);
					set_motor_right(150, MOTOR_FORWARD);
					_delay_ms(10);
					i++;
					if( i == 70){
						substate = 0;
						i = 0;
					}
					break;
				default: break;
			}
			break;
		case FOOTBALL_STATE_TOOCLOSE_RIGHT:
			// go back a little, turn around and go back to searching
			set_motor_left(200, MOTOR_BACKWARD);
			set_motor_right(200, MOTOR_BACKWARD);
			_delay_ms(300);
			set_motor_left(250, MOTOR_BACKWARD);
			set_motor_right(250, MOTOR_FORWARD);
			_delay_ms(400);
			set_motor_left(250, MOTOR_FORWARD);
			set_motor_right(250, MOTOR_BACKWARD);
			_delay_ms(50);
			set_motor_left(0, MOTOR_FORWARD);
			set_motor_right(0, MOTOR_FORWARD);
			_delay_ms(200);
			switch_state(FOOTBALL_STATE_SEARCH);
			break;
		case FOOTBALL_STATE_TOOCLOSE_LEFT:
			// go back a little, turn around and go back to searching
			set_motor_left(200, MOTOR_BACKWARD);
			set_motor_right(200, MOTOR_BACKWARD);
			_delay_ms(300);
			set_motor_left(250, MOTOR_FORWARD);
			set_motor_right(250, MOTOR_BACKWARD);
			_delay_ms(400);
			set_motor_left(250, MOTOR_BACKWARD);
			set_motor_right(250, MOTOR_FORWARD);
			_delay_ms(50);
			set_motor_left(0, MOTOR_FORWARD);
			set_motor_right(0, MOTOR_FORWARD);
			_delay_ms(200);
			switch_state(FOOTBALL_STATE_SEARCH);
			break;
		case FOOTBALL_STATE_SEE_BALL:
			//get distance to ball and decide how much to go forward
			//raise servo, start roller on full power
			//go forward a distance
			//lower servo, set roller to holding speed
			//if having enough balls
				//go to center and shoot
			//go to searching
			set_servo(SERVO_MIN);
			set_motor_left(0, MOTOR_FORWARD);
			set_motor_right(0, MOTOR_FORWARD);
			_delay_ms(500);
			
			set_roller(255, MOTOR_FORWARD);
			set_motor_left(200, MOTOR_FORWARD);
			set_motor_right(200, MOTOR_FORWARD);
			_delay_ms(700);
			
			set_roller(255, MOTOR_FORWARD);
			set_motor_left(160, MOTOR_BACKWARD);
			set_motor_right(160, MOTOR_BACKWARD);
			_delay_ms(500);
			
			set_roller(170, MOTOR_FORWARD);
			set_motor_left(0, MOTOR_FORWARD);
			set_motor_right(0, MOTOR_FORWARD);
			set_servo(SERVO_MAX-2);
			_delay_ms(1000);
			switch_state(FOOTBALL_STATE_SEARCH);
			break;
		case FOOTBALL_STATE_CENTER_AND_SHOOT:
			//check fototransistors and make adjustments until is centered
			//check the field sensor to be on right side
			//go to goal and shoot
			//turn around, go a distance
			//go to searching
			break;
		default: break;
	}
}


ISR(USART0_RX_vect)
{
	uint8_t command = UDR0;
	if(command == '$'){ // Read Sensors
		ana_set(ANA_SHARP_DOWN);
		ana_read();
		uint8_t jos = ana_read();
		
		ana_set(ANA_SHARP_UP);
		ana_read();
		uint8_t sus = ana_read();
		
		ana_set(ANA_FOTO_LEFT);
		ana_read();
		uint8_t lum_stanga = ana_read();
		
		ana_set(ANA_FOTO_RIGHT);
		ana_read();
		uint8_t lum_dreapta = ana_read();
		
		uint8_t stanga = dig_sharp(DIG_SHARP_LEFT);
		uint8_t dreapta = dig_sharp(DIG_SHARP_RIGHT);
		uint8_t field = dig_field();
		
		uint8_t threshold_jos = ( jos > sensor_thresholds.down) ? 1 : 0;
		uint8_t threshold_sus = ( sus > sensor_thresholds.up) ? 1 : 0;
		uint8_t threshold_stg = (lum_stanga > sensor_thresholds.left) \
			? 1 : 0;
		uint8_t threshold_drp = (lum_dreapta > sensor_thresholds.right) \
			? 1 : 0;
		
		uint8_t first_byte = (stanga<<7) | (dreapta<<6) | (field<<5) \
			| (threshold_jos<<4) | (threshold_sus<<3) | (threshold_stg<<2) \
			| (threshold_drp<<1);
		
		USART0_Transmit(first_byte);
		USART0_Transmit(jos);
		USART0_Transmit(sus);
		USART0_Transmit(lum_stanga);
		USART0_Transmit(lum_dreapta);

	}
	else if(command == '#'){ // Write thresholds
		uint8_t threshold_jos = USART0_Receive();
		uint8_t threshold_sus = USART0_Receive();
		uint8_t threshold_left = USART0_Receive();
		uint8_t threshold_right = USART0_Receive();
		sensor_thresholds.down = threshold_jos;
		sensor_thresholds.up = threshold_sus;
		sensor_thresholds.left = threshold_left;
		sensor_thresholds.right = threshold_right;
		store_config(&sensor_thresholds);
	}
	else if(command == '%'){ //Read thresholds
		USART0_Transmit(sensor_thresholds.down);
		USART0_Transmit(sensor_thresholds.up);
		USART0_Transmit(sensor_thresholds.left);
		USART0_Transmit(sensor_thresholds.right);
	}
}

__attribute__((noreturn))
int main()
{
	initleds();
	initadc();
	initmotors();
	USART0_Init();
	
	set_servo(SERVO_MAX-2);
	_delay_ms(100);
	uint8_t loaded_ok = load_config(&sensor_thresholds);
	if(!loaded_ok){
		set_led_field();
		sensor_thresholds.down = 100;
		sensor_thresholds.up = 100;
		sensor_thresholds.left = 75;
		sensor_thresholds.right = 75;
		store_config(&sensor_thresholds);
	}
	
	if(dig_stop()){
		USART0_Receive_sei();
		set_led_field();
		sei();
		for(;;);
	}
	
	set_led(LED1);
	_delay_ms(500);
	
	clear_led(LED1);
	set_led(LED2);
	_delay_ms(500);
	
	clear_led(LED2);
	set_led(LED3);
	_delay_ms(500);
	
	clear_led(LED3);
	set_led(LED4);
	_delay_ms(500);
	
	clear_led(LED4);
	_delay_ms(500);
	for(;;){
		football_logic();
	}
}
