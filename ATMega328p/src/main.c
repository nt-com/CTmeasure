/*********************************************************************
 * CTmeasure main function
 * Short Name: main
 * Author: nmt @ NT-COM
 * Date: 23.03.2020
 * Description: driver program
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [23.03.2020][nmt]: initial commit
 *********************************************************************/

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "twi.h"
#include "debug.h"
#include "uart.h"
#include "tim16.h"
#include "mlx90614.h"

/*********************************************************************
 * MACROS
 *********************************************************************/
/* set CPU frequency */
#ifndef F_CPU
	#warning "CPU FREQUENCY: F_CPU was undefined setting to 16 MHz"
	#define F_CPU 16000000UL
#endif

/* I calculated this using the formula listed below @timerinit,
	 I used a prescaler of 1024 to get this value. */
#define TIMER_5_HZ	0x061A

/*********************************************************************
 * TYPES
 *********************************************************************/

/*********************************************************************
 * GLOBALS
 *********************************************************************/

/* data buffer for reading sensor values and UART TX */
volatile uint8_t g_data_buffer[4] = { 0x00, 0x00, '\r', '\n' };

/*********************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************/
void debug_led_init(void);

/*********************************************************************
 * ISR
 *********************************************************************/
/* ISR triggered on timer1 compare match */
ISR (TIMER1_COMPA_vect) {

#ifdef DEBUG_MODE
	/* 	in debug mode the LED is toggled and an error code,
			if there is one, is sent per UART */
	PIND |= (1 << PIND6);
	uint8_t twi_error = mlx_read((uint8_t*)&g_data_buffer[0]);
	if(twi_error != OK) {
		debug_twi((uint8_t*)&g_data_buffer[0], twi_error);
		uart_send_string((uint8_t*)&g_data_buffer[0], 4);
	} else {
		uart_send_string((uint8_t*)&g_data_buffer[0], 4);
	}
#else
	mlx_read((uint8_t*)&g_data_buffer[0]);
	uart_send_string((uint8_t*)&g_data_buffer[0], 4);
#endif

	
}

/*********************************************************************
 * MAIN
 *********************************************************************/
int main(void) {

#ifdef DEBUG_MODE	/* toggle this in debug_cfg.h */ 
	/* stores the debug codes defined in debug.h for sending via UART */
	uint8_t tx_debug_buffer[3] = { 0x00 };
#endif

	/* timer setup */
	tim16_handle_t timer_handle = {0};
	timer_handle.prescaler_value = TIM16_PRE_1024;
	/*
		calculated as follows:
		compare_value = (Fcpu / (2*prescaler_value*timer_frequency)) - 1
		round to nearest integer @timerinit
	*/
	timer_handle.compare_value = TIMER_5_HZ;

	/* initialization */

#ifdef DEBUG_MODE
	/*** debug LED for timer functionality ***/
	debug_led_init();
#endif

	/** UART **/
	uart_init();

#ifdef DEBUG_MODE
	debug_get_message(&tx_debug_buffer[0], DEBUG_UART_INIT_DONE);
	uart_send_string(&tx_debug_buffer[0], DEBUG_MESSAGE_LENGTH);
#endif

	/** I2C **/
	twi_init();

#ifdef DEBUG_MODE
	debug_get_message(&tx_debug_buffer[0], DEBUG_I2C_INIT_DONE);
	uart_send_string(&tx_debug_buffer[0], DEBUG_MESSAGE_LENGTH);
#endif

	/** 16 bit timer **/
	
#ifdef DEBUG_MODE
	if(tim16_init(&timer_handle) == TIMER_SUCCESS) {
		debug_get_message(&tx_debug_buffer[0], DEBUG_TIMER_INIT_DONE);
	} else {
		debug_get_message(&tx_debug_buffer[0], DEBUG_TIMER_INIT_FAILED);
	}
	uart_send_string(&tx_debug_buffer[0], DEBUG_MESSAGE_LENGTH);
#else
	tim16_init(&timer_handle);
#endif 

	sei();	/* enable interrupts globally */

	while(1);

}

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
void debug_led_init(void) {
	/* pin D6 to output */
	DDRD = (1 << DDD6);
	/* set the pin to low */
	PIND &= ~(1 << PIND6);
}

/*********************************************************************
 * EOF
 *********************************************************************/

