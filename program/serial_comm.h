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
#define USART_BAUDRATE 38400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

extern uint8_t prog_mode;
extern IO_Data io;

#define CMD_PROGRAMMING 	0
#define CMD_TURN90_LEFT 	1
#define CMD_TURN90_RIGHT 	2
#define CMD_TURN_AROUND 	3
#define CMD_GO_BACK		4
#define CMD_VEER_LEFT 		5
#define CMD_VEER_RIGHT 	6
#define CMD_RAISE 			7
#define CMD_LOWER 			8
#define CMD_SUCK 			9
#define CMD_SPIT 			10
#define CMD_HOLD 			11
#define CMD_GET_BALL 		12
#define CMD_GO_FORWARD 	13
#define CMD_STOP 			14
#define CMD_STOP_ROLA		15
#define CMD_TURN10_LEFT	16
#define CMD_TURN10_RIGHT	17
#define CMD_CENTER_BALL	18
#define CMD_CENTER_LIGHT	19

#define CMD_READ_STOP_TIME 			30
#define CMD_READ_GET_BALL_POWER 		31
#define CMD_READ_GET_BALL_TIME 		32
#define CMD_READ_TURN90_POWER 		33
#define CMD_READ_TURN90_TIME 		34
#define CMD_READ_TURN180_POWER		35
#define CMD_READ_TURN180_TIME 		36
#define CMD_READ_GO_BACK_POWER 		37
#define CMD_READ_GO_BACK_TIME 		38
#define CMD_READ_VEER_POWER_MIN 		39
#define CMD_READ_VEER_POWER_MAX 		40
#define CMD_READ_GO_FORWARD_POWER 	41
#define CMD_READ_SUCK_POWER 		42
#define CMD_READ_SPIT_POWER 		43
#define CMD_READ_HOLD_POWER 		44
#define CMD_READ_SERVO_RAISE 		45
#define CMD_READ_SERVO_LOWER 		46
#define CMD_READ_SERVO_MOVE_TIME 	47
#define CMD_READ_TURN10_POWER		48
#define CMD_READ_TURN10_TIME		49

#define CMD_READ_SHARP_STANGA 	60
#define CMD_READ_SHARP_DREAPTA	61
#define CMD_READ_SHARP_JOS		62
#define CMD_READ_SHARP_SUS		63
#define CMD_READ_SENZ_TEREN 	64

#define CMD_SET_STOP_TIME 		80
#define CMD_SET_GET_BALL_POWER 	81
#define CMD_SET_GET_BALL_TIME	82
#define CMD_SET_TURN90_POWER 	83
#define CMD_SET_TURN90_TIME 	84
#define CMD_SET_TURN180_POWER 	85
#define CMD_SET_TURN180_TIME 	86
#define CMD_SET_GO_BACK_POWER 	87
#define CMD_SET_GO_BACK_TIME 	88
#define CMD_SET_VEER_POWER_MIN 	89
#define CMD_SET_VEER_POWER_MAX 	90
#define CMD_SET_GO_FORWARD_POWER	91
#define CMD_SET_SUCK_POWER 		92
#define CMD_SET_SPIT_POWER		93
#define CMD_SET_HOLD_POWER		94
#define CMD_SET_SERVO_RAISE 	95
#define CMD_SET_SERVO_LOWER 	96
#define CMD_SET_SERVO_MOVE_TIME	97
#define CMD_SET_TURN10_POWER	98
#define CMD_SET_TURN10_TIME		99



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
	uint8_t sharp1;
	if( (cmd >= CMD_PROGRAMMING) && (cmd < CMD_SET_STOP_TIME)){
		switch(cmd){
			case CMD_PROGRAMMING: prog_mode = 1;break;
			case CMD_TURN90_LEFT: turn90_left(); break;
			case CMD_TURN90_RIGHT: turn90_right(); break;
			case CMD_TURN_AROUND: turn_around();break;
			case CMD_GO_BACK: go_back();break;
			case CMD_VEER_LEFT: veer_left_nbk();break;
			case CMD_VEER_RIGHT: veer_right_nbk();break;
			case CMD_RAISE: raise(); break;
			case CMD_LOWER: lower(); break;
			case CMD_SUCK: rola_suck();break;
			case CMD_SPIT: rola_spit();break;
			case CMD_HOLD: rola_hold();break;
			case CMD_GET_BALL: get_ball(); break;
			case CMD_GO_FORWARD: go_forward_nbk(); break;
			case CMD_STOP: stop(); break;
			case CMD_STOP_ROLA: rola_stop();break;
			case CMD_TURN10_LEFT: turn10_left();break;
			case CMD_TURN10_RIGHT: turn10_right();break;
			case CMD_CENTER_BALL: center_ball(); break;
			case CMD_CENTER_LIGHT: center_light(); break;
			
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
			case CMD_READ_TURN10_POWER: USART0_Transmit(io.turn10_power);break;
			case CMD_READ_TURN10_TIME: USART0_Transmit(io.turn10_time);break;
			
			case CMD_READ_SHARP_STANGA:
				sharp1 = !(read_dig(SHARP_STANGA));
				USART0_Transmit(sharp1); 
				break;
			case CMD_READ_SHARP_DREAPTA:
				sharp1 = !(read_dig(SHARP_DREAPTA));
				USART0_Transmit(sharp1); 
				break;
			case CMD_READ_SHARP_JOS:
				sharp1 = read_adc_filtered(SHARP_JOS);
				USART0_Transmit(sharp1); 
				break;
			case CMD_READ_SHARP_SUS:
				sharp1 = read_adc_filtered(SHARP_SUS);
				USART0_Transmit(sharp1); 
				break;
			case CMD_READ_SENZ_TEREN:
				sharp1 = read_teren();
				USART0_Transmit(sharp1); 
				break;
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
			case CMD_SET_TURN10_POWER: io.turn10_power = val; break;
			case CMD_SET_TURN10_TIME: io.turn10_time = val; break;
		}
		store_config(&io);
	}
}