/*
 * gp_timer.c
 *
 *  Created on: Feb 4, 2020
 *      Author: nmt
 */

/**************************************
 * LIBRARIES
 *************************************/
#include "gp_timer.h"

/**************************************
 * GLOBALS
 *************************************/

/**************************************
 * INTERNAL FUNCTION PROTOTYPES
 *************************************/
static uint32_t gpt_get_counter(gp_timer_handle_t *handle);
static void gpt_timer_start(gp_timer_handle_t *handle);

/**************************************
 * INTERNAL FUNCTIONS
 *************************************/
static uint32_t gpt_get_counter(gp_timer_handle_t *handle) {
	return handle->pGPTIMx->CNT;
}

static void gpt_timer_start(gp_timer_handle_t *handle) {
	/* enable the counter in the counter control register */
	handle->pGPTIMx->CR1 |= (1 << 0);
}

/**************************************
 * EXTERNAL FUNCTIONS
 *************************************/

extern gp_timer_handle_t gpt_get_handle(void) {

	gp_timer_handle_t handle;
	memset(&handle, 0, sizeof(handle));
	handle.pGPTIMx = NULL;
	return handle;

}

extern uint8_t gpt_init(gp_timer_handle_t *handle) {

	uint32_t tempreg = 0x00;

	if(handle->pGPTIMx == TIM2) {
		TIM2_PCLK_EN();
	} else if(handle->pGPTIMx == TIM3) {
		TIM3_PCLK_EN();
	} else if(handle->pGPTIMx == TIM4) {
		TIM4_PCLK_EN();
	} else if(handle->pGPTIMx == TIM5) {
		TIM5_PCLK_EN();
	}

	if(handle->pGPTIMx == NULL) {
		return GP_TIMER_FAILURE;
	}

	/* set the reload value in ARR register */
	handle->pGPTIMx->ARR = handle->timer_config.reload_value;

	/* if interrupts are to be enabled, do the configuration */
	if(handle->timer_config.irq == GPT_IRQ_ENABLE) {
		handle->pGPTIMx->DIER |= (1 << 0);	/* update interrupt enable */
		handle->pGPTIMx->CR1 &= ~(1 << 1);	/* enable update generation */
	} else {
		/* disable update generation, only update on counter overflow */
		handle->pGPTIMx->CR1 |= (1 << 1);
	}

	/* configure the counter clock prescaler
	 * CLK_CNT = F_CK_psc / (PSC + 1) */
	tempreg = (((16000000U) / (handle->timer_config.timer_frequency)) - 1) & 0xffff;
	handle->pGPTIMx->PSC = tempreg;


	return GP_TIMER_SUCCESS;

}

extern uint8_t gpt_ioctl(gp_timer_handle_t *handle, uint8_t ioctl_code, uint32_t *buffer) {

	switch(ioctl_code) {

		case	GPT_IOCTL_GET_COUNTER_VALUE:
			*buffer = gpt_get_counter(handle);
			break;
		case	GPT_IOCTL_START_COUNTER:
			gpt_timer_start(handle);
			break;
		default:
			return GP_TIMER_FAILURE;
			break;

	} /* switch */

	return GP_TIMER_SUCCESS;

}

extern void gpt_interrupt_config(uint8_t IRQ_number, uint8_t enable) {

	if(enable == GPT_IRQ_ENABLE) {
		if(IRQ_number <= 31) {
			*NVIC_ISER0 |= ( 1 << IRQ_number );
		} else if(IRQ_number > 31 && IRQ_number < 64 ) {
			*NVIC_ISER1 |= ( 1 << (IRQ_number % 32) );
		} else if(IRQ_number >= 64 && IRQ_number < 96 ) {
			*NVIC_ISER2 |= ( 1 << (IRQ_number % 64) );
		}
	} else {
		if(IRQ_number <= 31) {
			*NVIC_ICER0 |= ( 1 << IRQ_number );
		} else if(IRQ_number > 31 && IRQ_number < 64 ) {
			*NVIC_ICER1 |= ( 1 << (IRQ_number % 32) );
		} else if(IRQ_number >= 64 && IRQ_number < 96 ) {
			*NVIC_ICER2 |= ( 1 << (IRQ_number % 64) );
		}
	} /* else */

}

extern void gpt_interrupt_handle(void) {

	*NVIC_ICPR0 |= (1 << IRQ_NO_TIM3);
	/* NOTE: add control for all timers */
	if( TIM3->SR & (1 << 0) ) {
		TIM3->SR &= ~(1 << 0);
		TIM3->SR &= ~(1 << 1);
		TIM3->SR &= ~(1 << 2);
		TIM3->SR &= ~(1 << 3);
		TIM3->SR &= ~(1 << 4);
		TIM3->EGR |= (1 << 0);
	}

}

/**************************************
 * EOF
 *************************************/
