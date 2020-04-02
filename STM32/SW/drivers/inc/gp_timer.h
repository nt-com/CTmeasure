/*
 * gp_timer.h
 *
 * Module Short Name: gpt
 *
 *  Created on: Feb 4, 2020
 *      Author: nmt
 */

#ifndef INC_GP_TIMER_H_
#define INC_GP_TIMER_H_

/**************************************
 * LIBRARIES
 *************************************/
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stm32f401re.h"

/**************************************
 * MACROS
 *************************************/
#define GP_TIMER_SUCCESS	0
#define GP_TIMER_FAILURE	1

#define GPT_IRQ_ENABLE		1
#define	 GPT_IRQ_DISABLE	0

#define GPT_MAX_RELOAD_VALUE	0xffff

#define GPT_CNT_TIMER_1000		1000U
#define	 GPT_CNT_TIMER_500		500U

/**************************************
 * TYPES
 *************************************/

typedef struct gp_timer_config {
	uint8_t channel;
	uint16_t reload_value;
	uint8_t irq;
	uint16_t timer_frequency;
} gp_timer_config_t;

typedef struct gp_timer_handle {
	gp_timer_regdef_t *pGPTIMx;
	gp_timer_config_t timer_config;
} gp_timer_handle_t;

const enum TIMER_NUMBERS {
	TIMER_2 = 2,
	TIMER_3 = 3,
	TIMER_4 = 4,
	TIMER_5 = 5
} TIM_NUMBER;

const enum GPT_CHANNEL {
	GPT_CHANNEL_1,
	GPT_CHANNEL_2,
	GPT_CHANNEL_3,
	GPT_CHANNEL_4
} TIM_CHAN_NUMBER;

const enum IOCTL_CMD {
	GPT_IOCTL_GET_COUNTER_VALUE,
	GPT_IOCTL_START_COUNTER
} TIMER_IOCTL_CMD;

/**************************************
 * FUNCTION PROTOTYPES
 *************************************/

/**
 * @brief returns a memset timer handle
 * @return a timer handle
 */
extern gp_timer_handle_t gpt_get_handle(void);

/**
 * @brief enables a timer peripheral
 * @param handle the handle to the TIM peripheral
 * @return 0 on success, 1 on failure
 */
extern uint8_t gpt_init(gp_timer_handle_t *handle);

/**
 * @brief ioctl for the timer peripheral
 * @param handle the handle to the timer peripheral
 * @param ioctl_code does the operation specified by the ioctl code
 * @param buffer contains data according to ioctl code
 * @return 0 on success, 1 on failure
 */
extern uint8_t gpt_ioctl(gp_timer_handle_t *handle, uint8_t ioctl_code, uint32_t *buffer);

/**
 * @brief configure timer interrupts
 * @param IRQ_number the IRQ number to enable or disable
 * @param enable if GP_IRQ_ENABLE, enable, if GP_IRQ_DISABLE, disable
 * @return void
 */
extern void gpt_interrupt_config(uint8_t IRQ_number, uint8_t enable);

/**
 * @brief handles a timer interrupt
 * @param void for now
 * @return void
 */
extern void gpt_interrupt_handle(void);

/**************************************
 * EOF
 *************************************/
#endif /* INC_GP_TIMER_H_ */
