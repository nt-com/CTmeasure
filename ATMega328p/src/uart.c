/*********************************************************************
 * UART Interface  Driver - C File
 * Short Name: uart
 * Author: nmt @ NT-COM
 * Date: 15.06.2019
 * Description: functions to use the UART on ATMega328p
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [15.09.2019][nmt]: initial commit
 *********************************************************************/

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include "uart.h"

/*********************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************/

/* NOTE: to follow along below, refer to the ATmega328p datasheet */

/* initializes the UART */
void uart_init() {
	
	/* the baudrate is expressed with 12 bits, so we have to 
	 * write the baudrate calculated in uart_cfg accordingly */
	/* the high register for the baudrate */
	UBRR0H = (uint8_t)(PRESCALE_VALUE>>8);
	/* the low register for the baudrate */
	UBRR0L = (uint8_t)(PRESCALE_VALUE);
	
	/* enable reception and sending */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	/* set the character size to 8 bits, meaning we send and receive 
	 * bytes */ 
	UCSR0C = ((1<<UCSZ00) | (1<<UCSZ01));
	
	/* take a look at clause 19.10.4, you will notice we are using
	 * 8N1 with the UART - 8 bits, no parity, 1 stop bit */	
}

/* for sending and receiving, check the comments of the macros 
 * in uart.h for a detailed summary */
 
/* sends a single character */
void uart_send(uint8_t ui8_data) {
	while(!UART_SEND_DONE);
	UDR0 = ui8_data;	
}

/* receives a single character */
uint8_t uart_recv() {
	uint8_t ui8_data = 0x00;
	while(!UART_RECV_DONE);
	ui8_data = UDR0;
	return ui8_data;
}
/* alternative receive function */
void uart_recv_alt(uint8_t *ui8_data) {
	while(!UART_RECV_DONE);
	*ui8_data = UDR0;
}

/* sends a string */
void uart_send_string(uint8_t *ui8_data, uint8_t len) {
	
	/* pass each byte to the send function, to send a string 
	 * we send it byte by byte */
	while(len) {
		uart_send( *(ui8_data++) );
		len--;
	}
}


/*********************************************************************
 * EOF
 *********************************************************************/
