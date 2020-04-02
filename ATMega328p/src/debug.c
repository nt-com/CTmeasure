/*********************************************************************
 * Debug Module - C File
 * Short Name: uart
 * Author: nmt @ NT-COM
 * Date: 23.03.2020
 * Description: functions for debugging
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [23.03.2020][nmt]: initial commit
 *********************************************************************/

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include "debug.h"

/*********************************************************************
 * GLOBALS
 *********************************************************************/
/* necessary delimiter for UART */
static uint8_t g_delimiter[2] = { '\r', '\n' };

/*********************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************/

extern void debug_get_message(uint8_t *buffer, uint8_t debug_code) {

	switch(debug_code) {
		case DEBUG_OK:
			*(buffer+0) = DEBUG_OK;
			break;
		case DEBUG_UART_INIT_DONE:
			*(buffer+0) = DEBUG_UART_INIT_DONE;
			break;
		case DEBUG_I2C_INIT_DONE: 
			*(buffer+0) = DEBUG_UART_INIT_DONE;
			break;
		case DEBUG_TIMER_INIT_DONE:
			*(buffer+0) = DEBUG_UART_INIT_DONE;
			break;
		case DEBUG_TIMER_INIT_FAILED:
			*(buffer+0) = DEBUG_TIMER_INIT_FAILED;
			break;
		default:
			*(buffer+0) = DEBUG_MODULE_ERROR;
			break;
	
	} /* switch */

	*(buffer+1) = g_delimiter[0];
	*(buffer+2) = g_delimiter[1];

}

extern void debug_twi(uint8_t *buffer, uint8_t debug_code) {
	*(buffer+0) = 'i';
	*(buffer+1) = debug_code;
	*(buffer+2) = g_delimiter[0];
	*(buffer+3) = g_delimiter[1];
}

/*********************************************************************
 * EOF
 *********************************************************************/
