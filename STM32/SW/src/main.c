/*
 * main.c
 *
 *	TX Only, use Pin A11 (14) on STM32
 *  Created on: Mar 21, 2020
 *      Author: nmt
 */

/***********************************************
 * LIBRARIES
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "gpio.h"
#include "usart.h"
#include "i2c.h"
#include "app.h"

/***********************************************
 * MACROS
 */
#define TRAP_FAILURE			while(1)

/** NOTE:
 * Have to scale down the clock somehow...
 * However, there must be a neater way to do this.
 * In future versions, I will modify the timer clocks
 * so a counter is not needed. */
#define DIVIDER_MEASUREMENTS	12000

/***********************************************
 * GLOBALS
 */
volatile uint16_t interrupt_count = 0x00;

/***********************************************
 * INTERRUPT SERVICE ROUTINE
 */
void TIM3_IRQHandler(void) {
	/* handle interrupt first */
	gpt_interrupt_handle();
	if(interrupt_count == DIVIDER_MEASUREMENTS) {
		/* now measure and send the data */
		app_mlx_measure(APP_MLX_DEVICE_ADDRESS);
		app_mlx_send();
		interrupt_count = 0;
	} else {
		interrupt_count++;
	}
}

/***********************************************
 * MAIN
 */
int main() {

	app_mlx_init();
	app_bluetooth_init();
	if( app_get_state() != APP_READY ) {
		TRAP_FAILURE;
	}

	app_start();

	while(1);
	/* everything happens in the interrupt service routine above */

}


/***********************************************
 * EOF
 */
