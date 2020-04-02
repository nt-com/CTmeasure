/*
 * app.h
 *
 *  Created on: Mar 22, 2020
 *      Author: nmt
 */

#ifndef INC_APP_H_
#define INC_APP_H_

/***********************************************
 * LIBRARIES
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "gpio.h"
#include "usart.h"
#include "i2c.h"
#include "gp_timer.h"

/***********************************************
 * MACROS
 */

/* temperature measurement related */
#define APP_TEMPERATURE_CONVERSION_CONSTANT 	0.02F
#define APP_KELVIN_CONVERSION 					273.15F
#define APP_I2C_BUFFER_SIZE	 					3
#define APP_MLX_DEVICE_ADDRESS 					0x5AU
#define APP_TEMPERATURE_REGISTER_ADDRESS		0x7

/* USART related */
#define APP_DELIMITER_LENGTH					2
#define APP_TEMPERATURE_STRING_LENGTH 			16
#define APP_USART_TX_MESSAGE_LENGTH	 			18

/***********************************************
 * GLOBALS
 */

/***********************************************
 * TYPES
 */
const enum {
	APP_NOT_INITIALIZED,
	APP_BLUETOOTH_INIT_DONE,
	APP_I2C_INIT_DONE,
	APP_READY
} APPLICATION_STATE; /* @APPLICATION_STATE */

/***********************************************
 * FUNCTION PROTOTYPES
 */

/**
 * @brief initializes bluetooth for sending temperature data
 * @param void
 * @return void
 */
extern void app_bluetooth_init(void);

/**
 * @brief initializes I2C for measuring temperature data
 * @param void
 * @return void
 */
extern void app_mlx_init(void);

/**
 * @brief starts the internal timer to take measurements, which starts the application
 * @param void
 * @return void
 */
extern void app_start(void);

/**
 * @brief returns the state of the application, should be APP_READY after init
 * @param void
 * @return application state, see @APPLICATION_STATE
 */
extern uint8_t app_get_state(void);

/**
 * @brief performs one temperature measurement
 * @param slave_address the I2C slave address of the temperature sensor
 * @return void
 */
extern void app_mlx_measure(uint8_t slave_address);

/**
 * @brief sends the measured data via bluetooth
 * @param void
 * @return void
 */
extern void app_mlx_send(void);

/***********************************************
 * EOF
 */
#endif /* INC_APP_H_ */
