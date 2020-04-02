/*********************************************************************
 * UART Interface  Driver - Header File
 * Short Name: uart
 * Author: nmt @ NT-COM
 * Date: 15.06.2019
 * Description: UART interfacing
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [15.09.2019][nmt]: initial commit
 *********************************************************************/

#ifndef UART_H
#define UART_H

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include <avr/io.h>
#include <stdint.h>

#include "uart_cfg.h"

/*********************************************************************
 * MACROS
 *********************************************************************/
 
/* NOTE: these macros are only used to make the code more readable */

/* check if uart is ready for sending  
 * uses the status and control register of UART A (UCSR0A) to check 
 * if the receive buffer is empty and ready to accept data for sending
 * (UDRE0) */
#define UART_SEND_DONE ( UCSR0A & (1 << UDRE0) )
/* check if data was received, using the status and control 
 * register, and the receive complete flag (RXC0)  */
#define UART_RECV_DONE ( UCSR0A & (1<<RXC0) )

/*********************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************/

/** 
 * @brief initializes the ATmega328p UART interface
 * @note: corresponds to 8N1 -> see uart.c
 * @return void 
 */
void uart_init(void);

/** 
 * @brief sends a single byte 
 * @param ui8_data the byte to send
 * @return void 
 */
void uart_send(uint8_t ui8_data);

/** 
 * @brief receives a single byte 
 * @return the received byte
 */
uint8_t uart_recv(void);

/* NOTE: this receive function may be better in some cases */
/**
 * @brief alternative receive function
 * @param ui8_data storage for the received byte
 * @return void
 */
void uart_recv_alt(uint8_t *ui8_data);

/** 
 * @brief sends multiple bytes of data
 * @param ui8_data data to send
 * @param len number of bytes to send
 * @return void 
 */
void uart_send_string(uint8_t *ui8_data, uint8_t len);



/*********************************************************************
 * EOF
 *********************************************************************/
#endif

