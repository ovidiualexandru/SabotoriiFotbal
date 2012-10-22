#pragma once
#include <stdint.h>
#include <avr/io.h>
#include "defines.h"
#include "config.h"

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

#define GET_BALL_POWER io.get_ball_power
#define GET_BALL_TIME io.get_ball_time
inline static void get_ball()
{
	set_motor_left(GET_BALL_POWER, MOTOR_FORWARD);
	set_motor_right(GET_BALL_POWER, MOTOR_FORWARD);
	rola_suck();
	wait_nbk(GET_BALL_TIME);
	rola_hold();
	stop();
}

#define TURN90_POWER io.turn90_power
#define TURN90_TIME io.turn90_time
inline static void turn_left()
{
	stop();
	set_motor_left(TURN90_POWER, MOTOR_BACKWARD);
	set_motor_right(TURN90_POWER, MOTOR_FORWARD);
	wait_nbk(TURN90_TIME);
	stop();
}

inline static void turn_right()
{
	stop();
	set_motor_left(TURN90_POWER, MOTOR_FORWARD);
	set_motor_right(TURN90_POWER, MOTOR_BACKWARD);
	wait_nbk(TURN90_TIME);
	stop();
}

#define TURN180_POWER io.turn180_power
#define TURN180_TIME io.turn180_time
inline static void turn_around()
{
	stop();
	set_motor_left(TURN180_POWER, MOTOR_FORWARD);
	set_motor_right(TURN180_POWER, MOTOR_BACKWARD);
	wait_nbk(TURN180_TIME);
	stop();
}


#define GO_BACK_TIME io.go_back_time
#define GO_BACK_POWER io.go_back_power
inline static void go_back()
{
	set_motor_left(GO_BACK_POWER, MOTOR_BACKWARD);
	set_motor_right(GO_BACK_POWER, MOTOR_BACKWARD);
	wait_nbk(GO_BACK_TIME);
	stop();
}

#define VEER_POWER_MIN io.veer_power_min
#define VEER_POWER_MAX io.veer_power_max
inline static void veer_left_nbk()
{
	set_motor_left(VEER_POWER_MIN, MOTOR_FORWARD);
	set_motor_right(VEER_POWER_MAX, MOTOR_FORWARD);
}

inline static void veer_right_nbk()
{
	set_motor_left(VEER_POWER_MAX, MOTOR_FORWARD);
	set_motor_right(VEER_POWER_MIN, MOTOR_FORWARD);
}

#define GO_FORWARD_POWER io.go_forward_power
inline static void go_forward_nbk()
{
	set_motor_left(GO_FORWARD_POWER, MOTOR_FORWARD);
	set_motor_right(GO_FORWARD_POWER, MOTOR_FORWARD);
}