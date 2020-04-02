/*********************************************************************
 * Debug Module - Header File
 * Short Name: debug
 * Author: nmt @ NT-COM
 * Date: 23.03.2020
 * Description: DEBUG interfacing
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [23.03.2020][nmt]: initial commit
 *********************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include <stdint.h>
#include "debug_cfg.h"

/*********************************************************************
 * MACROS
 *********************************************************************/
#define DEBUG_MESSAGE_LENGTH 3
 
/*********************************************************************
 * TYPES
 *********************************************************************/
const enum {
	DEBUG_OK,
	DEBUG_UART_INIT_DONE,
	DEBUG_I2C_INIT_DONE,
	DEBUG_TIMER_INIT_FAILED,
	DEBUG_TIMER_INIT_DONE,
	DEBUG_MODULE_ERROR
} DEBUG_CODES; 

/*********************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************/

/**
 * @brief get a debug message for a specific initialization step
 * @param buffer buffer to load the debug message to
 * @param debug_code debug code to load the message for
 * @return void
 *
 */
extern void debug_get_message(uint8_t *buffer, uint8_t debug_code);

/**
 * @brief get a debug message for TWI on error
 * @param buffer buffer to load the debug message to
 * @param debug_code debug code to load the message for
 * @return void
 *
 */
extern void debug_twi(uint8_t *buffer, uint8_t debug_code);

/*********************************************************************
 * EOF
 *********************************************************************/
#endif

