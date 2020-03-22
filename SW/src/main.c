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
#define TRAP_FAILURE	while(1)

/***********************************************
 * FUNCTION PROTOTYPES
 */
void delay(uint32_t n);

/***********************************************
 * MAIN
 */
int main() {

	app_mlx_init();
	app_bluetooth_init();
	if( app_get_state() != APP_READY ) {
		TRAP_FAILURE;
	}

	while(1) {
		app_mlx_measure(APP_MLX_DEVICE_ADDRESS);
		app_mlx_send();
		delay(250000);
		delay(250000);
		delay(250000);
	}
}

/***********************************************
 * FUNCTIONS
 */
void delay(uint32_t n) {
	uint32_t i = 0x00;
	for(i = 0; i < n; i++);
}


/***********************************************
 * EOF
 */
