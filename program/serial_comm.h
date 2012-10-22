#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "defines.h"
#include "motor_control.h"
#include "eeprom_data.h"

#define USART0_DDR DDRD
#define USART0_TX_BIT PD1
#define USART0_RX_BIT PD0
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

extern uint8_t prog_mode;
extern IO_Data io;

#define CMD_PROGRAMMING 0x00
#define CMD_TURN_LEFT 0x01
#define CMD_TURN_RIGHT 0x02
#define CMD_TURN_AROUND 0x03
#define CMD_VEER_LEFT 0x04
#define CMD_VEER_RIGHT 0x05
#define CMD_RAISE 0x06
#define CMD_LOWER 0x07
#define CMD_SUCK 0x08
#define CMD_SPIT 0x09
#define CMD_HOLD 0x0A

#define CMD_READ_STOP_TIME 0x0B
#define CMD_READ_GET_BALL_POWER 0x0C
#define CMD_READ_GET_BALL_TIME 0x0D
#define CMD_READ_TURN90_POWER 0x0E
#define CMD_READ_TURN90_TIME 0x0F
#define CMD_READ_TURN180_POWER 0x10
#define CMD_READ_TURN180_TIME 0x11
#define CMD_READ_GO_BACK_POWER 0x12
#define CMD_READ_GO_BACK_TIME 0x13
#define CMD_READ_VEER_POWER_MIN 0x14
#define CMD_READ_VEER_POWER_MAX 0x15
#define CMD_READ_GO_FORWARD_POWER 0x16
#define CMD_READ_SUCK_POWER 0x17
#define CMD_READ_SPIT_POWER 0x18
#define CMD_READ_HOLD_POWER 0x19
#define CMD_READ_SERVO_RAISE 0x1A
#define CMD_READ_SERVO_LOWER 0x1B
#define CMD_READ_SERVO_MOVE_TIME 0x1C

#define CMD_SET_STOP_TIME 0x1D
#define CMD_SET_GET_BALL_POWER 0x1E
#define CMD_SET_GET_BALL_TIME 0x1F
#define CMD_SET_TURN90_POWER 0x20
#define CMD_SET_TURN90_TIME 0x21
#define CMD_SET_TURN180_POWER 0x22
#define CMD_SET_TURN180_TIME 0x23
#define CMD_SET_GO_BACK_POWER 0x24
#define CMD_SET_GO_BACK_TIME 0x25
#define CMD_SET_VEER_POWER_MIN 0x26
#define CMD_SET_VEER_POWER_MAX 0x27
#define CMD_SET_GO_FORWARD_POWER 0x28
#define CMD_SET_SUCK_POWER 0x29
#define CMD_SET_SPIT_POWER 0x2A
#define CMD_SET_HOLD_POWER 0x2B
#define CMD_SET_SERVO_RAISE 0x2C
#define CMD_SET_SERVO_LOWER 0x2D
#define CMD_SET_SERVO_MOVE_TIME 0x2E



void USART0_Init()
{
	USART0_DDR |= _BV(USART0_TX_BIT) | _BV(USART0_RX_BIT); // outputs for TX
	// PORTD |= _BV(PD0) | _BV(PD1); //Set to 1, why?
	UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01); // Use 8- bit character sizes
	UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8- bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = BAUD_PRESCALE; // Load lower 8- bits of the baud rate value into the low byte of the UBRR register
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0); // Turn on the transmission and reception circuitry and the receive interrupt
}

static void USART0_Transmit(uint8_t data)
{
	// Wait for empty transmit buffer
	// while( !( UCSR0A & (1<<UDRE0)) );
	loop_until_bit_is_set(UCSR0A, UDRE0);
	// Put data into buffer, sends the data
	UDR0 = data;
}

static uint8_t USART0_Receive()
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}
// ISR(USART0__RX_vect)
ISR(USART0_RX_vect)
{
	uint8_t cmd = UDR0;
	if( (cmd >= CMD_PROGRAMMING) && (cmd <= CMD_READ_SERVO_MOVE_TIME)){
		switch(cmd){
			case CMD_PROGRAMMING: prog_mode = 1;break;
			case CMD_TURN_LEFT: turn_left(); break;
			case CMD_TURN_RIGHT: turn_right(); break;
			case CMD_TURN_AROUND: turn_around();break;
			case CMD_VEER_LEFT: veer_left_nbk();break;
			case CMD_VEER_RIGHT: veer_right_nbk();break;
			case CMD_RAISE: raise(); break;
			case CMD_LOWER: lower(); break;
			case CMD_SUCK: rola_suck();break;
			case CMD_SPIT: rola_spit();break;
			case CMD_HOLD: rola_hold();break;
			case CMD_READ_STOP_TIME: USART0_Transmit(io.stop_time);break;
			case CMD_READ_GET_BALL_POWER: USART0_Transmit(io.get_ball_power);break;
			case CMD_READ_GET_BALL_TIME: USART0_Transmit(io.get_ball_time);break;
			case CMD_READ_TURN90_POWER: USART0_Transmit(io.turn90_power);break;
			case CMD_READ_TURN90_TIME: USART0_Transmit(io.turn90_time);break;
			case CMD_READ_TURN180_POWER: USART0_Transmit(io.turn180_power);break;
			case CMD_READ_TURN180_TIME: USART0_Transmit(io.turn180_time);break;
			case CMD_READ_GO_BACK_POWER: USART0_Transmit(io.go_back_power);break;
			case CMD_READ_GO_BACK_TIME: USART0_Transmit(io.go_back_time);break;
			case CMD_READ_VEER_POWER_MIN: USART0_Transmit(io.veer_power_min);break;
			case CMD_READ_VEER_POWER_MAX: USART0_Transmit(io.veer_power_max);break;
			case CMD_READ_GO_FORWARD_POWER: USART0_Transmit(io.go_forward_power);break;
			case CMD_READ_SUCK_POWER: USART0_Transmit(io.suck_power);break;
			case CMD_READ_SPIT_POWER: USART0_Transmit(io.spit_power);break;
			case CMD_READ_HOLD_POWER: USART0_Transmit(io.hold_power);break;
			case CMD_READ_SERVO_RAISE: USART0_Transmit(io.servo_raise);break;
			case CMD_READ_SERVO_LOWER: USART0_Transmit(io.servo_lower);break;
			case CMD_READ_SERVO_MOVE_TIME: USART0_Transmit(io.servo_move_time);break;
		}
	}
	else{
		uint8_t val = USART0_Receive();
		switch(cmd){
			case CMD_SET_STOP_TIME: io.stop_time = val; break;
			case CMD_SET_GET_BALL_POWER: io.get_ball_power = val; break;
			case CMD_SET_GET_BALL_TIME: io.get_ball_time = val; break;
			case CMD_SET_TURN90_POWER: io.turn90_power = val; break;
			case CMD_SET_TURN90_TIME: io.turn90_time = val; break;
			case CMD_SET_TURN180_POWER: io.turn180_power = val; break;
			case CMD_SET_TURN180_TIME: io.turn180_time = val; break;
			case CMD_SET_GO_BACK_POWER: io.go_back_power = val; break;
			case CMD_SET_GO_BACK_TIME: io.go_back_time = val; break;
			case CMD_SET_VEER_POWER_MIN: io.veer_power_min = val; break;
			case CMD_SET_VEER_POWER_MAX: io.veer_power_max = val; break;
			case CMD_SET_GO_FORWARD_POWER: io.go_forward_power = val; break;
			case CMD_SET_SUCK_POWER: io.suck_power = val; break;
			case CMD_SET_SPIT_POWER: io.spit_power = val; break;
			case CMD_SET_HOLD_POWER: io.hold_power = val; break;
			case CMD_SET_SERVO_RAISE: io.servo_raise = val; break;
			case CMD_SET_SERVO_LOWER: io.servo_lower = val; break;
			case CMD_SET_SERVO_MOVE_TIME: io.servo_move_time = val; break;
		}
		store_config(&io);
	}
}