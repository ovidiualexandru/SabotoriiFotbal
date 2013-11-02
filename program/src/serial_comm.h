#pragma once

#include <stdint.h>

/*****************************************
USART Library
serial_comm.h
Uses: USART0

Configures and uses the USART for the 324P.
This version uses only the USART0 and functions
are blocking (no interrupts).
*****************************************/
#define USART_BAUDRATE 38400

#define USART0_DDR DDRD
#define USART0_TX_BIT PD1
#define USART0_RX_BIT PD0
/****************************************/

/*
 * Initialize the USART peripheral without interrupts.
 */
void USART0_Init();

/*
 * Transmit a byte. Blocking function.
 */
void USART0_Transmit(uint8_t data);

/*
 * Receive a byte. Blocking function - if byte is not received, waits 
 * indefinetely.
 */
uint8_t USART0_Receive();
