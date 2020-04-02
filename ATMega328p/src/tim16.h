/*********************************************************************
 * 16 bit Timer - Header File
 * Short Name: tim16
 * Author: nmt @ NT-COM
 * Date: 23.03.2020
 * Description: Definitions and functions for the 16 bit timer
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [23.03.2020][nmt]: initial commit
 *********************************************************************/

#ifndef TIM16_H
#define TIM16_H

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include <avr/io.h>
#include <stdint.h>
#include "tim16_cfg.h"

/*********************************************************************
 * MACROS
 *********************************************************************/
#define TIMER_SUCCESS	0
#define TIMER_FAILURE 1
 
/*********************************************************************
 * TYPES
 *********************************************************************/
/* @handle */
typedef struct tim16_handle {
	uint16_t compare_value;
	uint16_t prescaler_value;
} tim16_handle_t;

const enum {
	TIM16_STOP_TIMER,
	TIM16_PRE_1,				/* prescaler value of 1 */
	TIM16_PRE_8,
	TIM16_PRE_64,
	TIM16_PRE_256,
	TIM16_PRE_1024
} TIM16_PRESCALER_VALUES;

/*********************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************/

/**
 * @brief get a debug message for a specific event
 * @param p_tim16_handle configuration values for the timer @handle
 * @param debug_code debug code to load the message for
 * @return void
 */
extern uint8_t tim16_init(tim16_handle_t *p_tim16_handle);

/*********************************************************************
 * EOF
 *********************************************************************/
#endif

