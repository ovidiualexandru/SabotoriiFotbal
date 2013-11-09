#include <avr/io.h>
#include <stdint.h>
#include "serial_comm.h"

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void USART0_Init()
{
	USART0_DDR |= _BV(USART0_TX_BIT) | _BV(USART0_RX_BIT); // outputs for TX
	// PORTD |= _BV(PD0) | _BV(PD1); //Set to 1, why?
	UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01); // Use 8- bit character sizes
	UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8- bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = BAUD_PRESCALE; // Load lower 8- bits of the baud rate value into the low byte of the UBRR register
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0) /*| _BV(RXCIE0)*/; // Turn on the transmission and reception circuitry and the receive interrupt
}

void USART0_Transmit(uint8_t data)
{
	// Wait for empty transmit buffer
	// while( !( UCSR0A & (1<<UDRE0)) );
	loop_until_bit_is_set(UCSR0A, UDRE0);
	// Put data into buffer, sends the data
	UDR0 = data;
}

uint8_t USART0_Receive()
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

void USART0_Receive_sei()
{
	UCSR0B |= _BV(RXCIE0);
}
