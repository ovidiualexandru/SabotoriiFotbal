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

uint8_t see_ball(uint8_t up, uint8_t down)
{
	uint8_t ret = 0;
	if( down > up && (down - up) > 10) ret = 1; // condition should be using thresh 
	return ret;
}

#define FOOTBALL_STATE_SEARCH 0
#define FOOTBALL_STATE_DIGITAL_SHARP 1
#define FOOTBALL_STATE_SEE_BALL 2
#define FOOTBALL_STATE_CENTER_AND_SHOOT 3

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

void football_logic()
{
	static uint8_t state = FOOTBALL_STATE_SEARCH;
	switch(state){
		case FOOTBALL_STATE_SEARCH:
			//go forward a little
			//check digital sharps
				//if set, is next to wall, go somewhere else
			//check analog sharps
				//if can see ball, goto state SEE_BALL or just get it
				if(dig_sharp(DIG_SHARP_LEFT) || dig_sharp(DIG_SHARP_RIGHT)){
					turn_around();
				}
			go_forward();
			ana_set(ANA_SHARP_DOWN);
			ana_read();
			uint8_t jos = ana_read();
		
			ana_set(ANA_SHARP_UP);
			ana_read();
			uint8_t sus = ana_read();
			uint8_t seeing_ball = see_ball(sus, jos);
			
			if(seeing_ball){
				get_ball();
			}
			
			break;
		case FOOTBALL_STATE_DIGITAL_SHARP:
			// go back a little, turn around and go back to searching
			break;
		case FOOTBALL_STATE_SEE_BALL:
			//get distance to ball and decide how much to go forward
			//raise servo, start roller on full power
			//go forward a distance
			//lower servo, set roller to holding speed
			//if having enough balls
				//go to center and shoot
			//go to searching
			break;
		case FOOTBALL_STATE_CENTER_AND_SHOOT:
			//check fototransistors and make adjustments until is centered
			//check the field sensor to be on right side
			//go to goal and shoot
			//turn around, go a distance
			//go to searching
			break;
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
