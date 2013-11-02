#pragma once

#include <stdint.h>
#include "defines.h"

/*****************************************
Motor Control
motors.h
Uses: Timer 0, Timer 1 and Timer 2.

DC motors and servo control.
*****************************************/
#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 0
#define SERVO_MAX 39
#define SERVO_MIN 20
/****************************************/

/*
 * Initialize all motors - Timer 0,1, 2 left/right dc motors, the roll dc motor 
 * and the servo
 */
void initmotors();

/*
 * Set the left motor power and direction
 */
void set_motor_left(uint8_t value, uint8_t dir);

/*
 * Set the right motor power and direction
 */
void set_motor_right(uint8_t value, uint8_t dir);

/*
 * Set the roll motor power and direction. Forward = hold/pull ball, 
 * Backward = push ball
 */
void set_rola(uint8_t value, uint8_t dir);

/*
 * Set the servo position. Valid positions between 20 and 39.
 * The function does not set the register if invalid value is given ( pulse
 * should be between 1 and 2 ms). If invalid value is given, function return 0, 
 * else return 1.
 */
uint8_t set_servo(uint8_t pos);