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
uint8_t field;

/*****************************************
Analog reading 
*****************************************/
uint8_t ana_read_chan(uint8_t channel)
{
	ana_set(channel);
	ana_read();
	uint8_t ret = ana_read();
	return ret;
}

uint8_t ana_read_filtered(uint8_t channel)
{
	uint8_t j;
	ana_set(channel);
	ana_read();
	uint16_t acc = 0;
	for( j = 0; j < 4; j++){
		acc += ana_read();
		}
	uint8_t ret = acc >> 2;
	return ret;
}
/****************************************/

/*****************************************
Interpreting
*****************************************/
#define BD_NOWALL_NOBALL 0
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
	if( up > 80 ) wall = BD_MEDIUMWALL;
	if( up > 150 ) wall = BD_CLOSEWALL;
	
	if( (down > up) && (down - up > 50)){
		if( wall == BD_NOWALL ) ball = BD_MEDIUMBALL;
		if( wall == BD_MEDIUMWALL ) ball = BD_CLOSEBALL;
		if( wall == BD_CLOSEWALL ) ball = BD_CLOSEBALL;
	}
	ret = wall | ball;
	if( wall == BD_CLOSEWALL && up > down) ret = BD_TOOCLOSE;
	return ret;
}

#define LD_NOLIGHT 0x00
#define LD_FARLIGHT 0x01
#define LD_MEDIUMLIGHT 0x02
#define LD_CLOSELIGHT 0x03
#define LD_GOALLIGHT 0x04

uint8_t light_detection(uint8_t left, uint8_t right)
{
	uint8_t ret;
	uint8_t lleft = LD_NOLIGHT, lright = LD_NOLIGHT;
	
	uint8_t sleft = sensor_thresholds.ambient_left;
	uint8_t sright = sensor_thresholds.ambient_right;
	
	
	if(left > sleft){
		if( left - sleft > 30) lleft = LD_FARLIGHT;
		if( left - sleft > 60) lleft = LD_MEDIUMLIGHT;
		if( left - sleft > 90) lleft = LD_CLOSELIGHT;
		if( left > 200) lleft = LD_GOALLIGHT;
	}
	if(right > sright){
		if( right - sright > 30) lright = LD_FARLIGHT;
		if( right - sright > 60) lright= LD_MEDIUMLIGHT;
		if( right - sright > 90) lright= LD_CLOSELIGHT;
		if( right > 200) lright= LD_GOALLIGHT;
	}
	ret = (lleft<<4) | lright;
	return ret;
}
/****************************************/

/*****************************************
Hard-coded actions
*****************************************/
void get_ball()
{
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
	_delay_ms(500);
}

void too_close_right()
{
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
}

void too_close_left()
{
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
}

void shoot()
{
	set_servo(SERVO_MIN);
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(500);
	set_roller(255, MOTOR_BACKWARD);
	_delay_ms(2000);
	set_motor_left(250, MOTOR_BACKWARD);
	set_motor_right(250, MOTOR_BACKWARD);
	_delay_ms(200);
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	set_roller(100, MOTOR_FORWARD);
	_delay_ms(300);
	set_servo(SERVO_MAX-2);
	_delay_ms(300);
	set_motor_left(200, MOTOR_FORWARD);
	set_motor_right(200, MOTOR_BACKWARD);
	_delay_ms(300);
	set_motor_left(0, MOTOR_FORWARD);
	set_motor_right(0, MOTOR_FORWARD);
	_delay_ms(300);
}
/****************************************/

/*****************************************
Football logic
*****************************************/
typedef void (*State)(uint8_t, uint8_t);
State state;

void football_logic()
{
	uint8_t down, up, left, right;
	
	down = ana_read_filtered(ANA_SHARP_DOWN);			
	up = ana_read_filtered(ANA_SHARP_UP);
	left = ana_read_filtered(ANA_FOTO_LEFT);
	right = ana_read_filtered(ANA_FOTO_RIGHT);
	uint8_t ballpos = ball_detection(up, down);
	uint8_t lightpos = light_detection(left, right);

	if(dig_sharp(DIG_SHARP_LEFT) || ballpos == BD_TOOCLOSE){
		too_close_left();
	}
	else if( dig_sharp(DIG_SHARP_RIGHT)){
		too_close_right();
	}
	state(ballpos, lightpos);
}
/****************************************/


/*****************************************
State machine logic
*****************************************/
#define SCAN_BASE_SPEED 120
#define SCAN_GO_FORWARD_SPEED 130

#define SCAN_LEFT_TIME 100
#define SCAN_RIGHT_TIME 200
#define SCAN_STRAIGHTEN_TIME 120
#define SCAN_GO_FORWARD_TIME 100

#define reset_state() substate = 0; \
	i = 0; \
	set_motor_left(0, MOTOR_FORWARD); \
	set_motor_right(0, MOTOR_FORWARD); \
	_delay_ms(500)

void state_search(uint8_t ballpos, uint8_t lightpos)
{
	static uint8_t substate = 0;
	static uint8_t i = 0;
	static uint8_t balls = 0;
	//Set state leds
	set_led(LED1);
	clear_led(LED2);
	//Set sensor leds
	if( (ballpos & 0x0F) != BD_NOBALL) set_led(LED3);
	else clear_led(LED3);
	if( (ballpos >> 4) != BD_NOWALL) set_led(LED4);
	else clear_led(LED4);
	if( (ballpos & 0x0F) != BD_NOBALL){
		get_ball();
		balls++;
		reset_state();
		// TODO : complete code
	}
	switch(substate){
		case 0: //SCAN LEFT
			set_motor_left(SCAN_BASE_SPEED, MOTOR_BACKWARD);
			set_motor_right(SCAN_BASE_SPEED, MOTOR_FORWARD);
			_delay_ms(10);
			i++;
			if( i == SCAN_LEFT_TIME){
				substate = 1;
				i = 0;
			}
			break;
		case 1: //SCAN RIGHT
			set_motor_left(SCAN_BASE_SPEED, MOTOR_FORWARD);
			set_motor_right(SCAN_BASE_SPEED, MOTOR_BACKWARD);
			_delay_ms(10);
			i++;
			if( i == SCAN_RIGHT_TIME){
				substate = 2;
				i = 0;
			}
			break;
		case 2: //STRAIGHTEN
			set_motor_left(SCAN_BASE_SPEED, MOTOR_BACKWARD);
			set_motor_right(SCAN_BASE_SPEED, MOTOR_FORWARD);
			_delay_ms(10);
			i++;
			if( i == SCAN_STRAIGHTEN_TIME){
				substate = 3;
				i = 0;
			}
			break;
		case 3: //GO FORWARD
			set_motor_left(SCAN_GO_FORWARD_SPEED, MOTOR_FORWARD);
			set_motor_right(SCAN_GO_FORWARD_SPEED, MOTOR_FORWARD);
			_delay_ms(10);
			i++;
			if( i == SCAN_GO_FORWARD_TIME){
				substate = 0;
				i = 0;
			}
			break;
		default: break;
	}
}

void state_goal(uint8_t ballpos, uint8_t lightpos)
{
	static uint8_t substate = 0;
	static uint8_t i = 0;
	uint8_t left = lightpos >> 4;
	uint8_t right = lightpos & 0x0F;
	//Set state leds
	clear_led(LED1);
	set_led(LED2);
	//Set sensor leds
	if(left != LD_NOLIGHT) set_led(LED3);
	else clear_led(LED3);
	if(right!= LD_NOLIGHT) set_led(LED4);
	else clear_led(LED4);
	if( ((left!= LD_NOLIGHT) || (right != LD_NOLIGHT))){
		if( (left == LD_GOALLIGHT) && (right == LD_GOALLIGHT) ){
			substate = 8;
			}
		else if(substate <= 3){
			substate = 4;
		}
	}
	else if( substate > 3){
		substate = 0;
	}
	switch(substate){
		case 0: //SCAN LEFT
			set_motor_left(SCAN_BASE_SPEED, MOTOR_BACKWARD);
			set_motor_right(SCAN_BASE_SPEED, MOTOR_FORWARD);
			_delay_ms(10);
			i++;
			if( i == SCAN_LEFT_TIME){
				substate = 1;
				i = 0;
			}
			break;
		case 1: //SCAN RIGHT
			set_motor_left(SCAN_BASE_SPEED, MOTOR_FORWARD);
			set_motor_right(SCAN_BASE_SPEED, MOTOR_BACKWARD);
			_delay_ms(10);
			i++;
			if( i == SCAN_RIGHT_TIME){
				substate = 2;
				i = 0;
			}
			break;
		case 2: //STRAIGHTEN
			set_motor_left(SCAN_BASE_SPEED, MOTOR_BACKWARD);
			set_motor_right(SCAN_BASE_SPEED, MOTOR_FORWARD);
			_delay_ms(10);
			i++;
			if( i == SCAN_STRAIGHTEN_TIME){
				substate = 3;
				i = 0;
			}
			break;
		case 3: //GO FORWARD
			set_motor_left(SCAN_GO_FORWARD_SPEED, MOTOR_FORWARD);
			set_motor_right(SCAN_GO_FORWARD_SPEED, MOTOR_FORWARD);
			_delay_ms(10);
			i++;
			if( i == SCAN_GO_FORWARD_TIME){
				substate = 0;
				i = 0;
			}
			break;
		case 4:
			if( left > right){
				substate = 5;
			}
			else if( right > left){
				substate = 6;
			}
			else{ // left == right
				substate = 7;
			}
			break;
		case 5: //SCAN LIGHT LEFT
			set_motor_left(80, MOTOR_FORWARD);
			set_motor_right(130, MOTOR_FORWARD);
			_delay_ms(30);
			substate = 4;
			break;
		case 6: //SCAN LIGHT RIGHT
			set_motor_left(130, MOTOR_FORWARD);
			set_motor_right(80, MOTOR_FORWARD);
			_delay_ms(30);
			substate = 4;
			break;
		case 7: //GO FORWARD
			set_motor_left(150, MOTOR_FORWARD);
			set_motor_right(150, MOTOR_FORWARD);
			_delay_ms(30);
			substate = 4;
			break;
		case 8:
			shoot();
			break;
		default: break;
	}
	
}
/****************************************/

ISR(USART0_RX_vect)
{
	uint8_t command = UDR0;
	if(command == '$'){ // Read Sensors
		uint8_t jos = ana_read_filtered(ANA_SHARP_DOWN);
		uint8_t sus = ana_read_filtered(ANA_SHARP_UP);
		
		ana_set(ANA_FOTO_LEFT);
		ana_read();
		uint8_t lum_stanga = ana_read();
		
		ana_set(ANA_FOTO_RIGHT);
		ana_read();
		uint8_t lum_dreapta = ana_read();
		
		uint8_t stanga = dig_sharp(DIG_SHARP_LEFT);
		uint8_t dreapta = dig_sharp(DIG_SHARP_RIGHT);
		uint8_t lfield = dig_field();
		
		uint8_t threshold_stg = (lum_stanga > sensor_thresholds.ambient_left) \
			? 1 : 0;
		uint8_t threshold_drp = (lum_dreapta > sensor_thresholds.ambient_right) \
			? 1 : 0;
		
		uint8_t first_byte = (stanga<<7) | (dreapta<<6) | (lfield<<5) \
			| (1<<4) | (1<<3) | (threshold_stg<<2) \
			| (threshold_drp<<1);
		
		USART0_Transmit(first_byte);
		USART0_Transmit(jos);
		USART0_Transmit(sus);
		USART0_Transmit(lum_stanga);
		USART0_Transmit(lum_dreapta);

	}
	else if(command == '#'){ // Write thresholds
		uint8_t threshold_left = USART0_Receive();
		uint8_t threshold_right = USART0_Receive();
		sensor_thresholds.ambient_left= threshold_left;
		sensor_thresholds.ambient_right = threshold_right;
		store_config(&sensor_thresholds);
	}
	else if(command == '%'){ //Read thresholds
		USART0_Transmit(sensor_thresholds.ambient_left);
		USART0_Transmit(sensor_thresholds.ambient_right);
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
	sensor_thresholds.ambient_left = 30;
	sensor_thresholds.ambient_right = 50;
	
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
	
	field = dig_field();
	state = state_goal;
	for(;;){
		football_logic();
	}
}
