/*
 * gpio.h
 *
 *	Short Name: IO
 *
 *  Created on: Feb 5, 2020
 *      Author: nmt
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

/**************************************
 * LIBRARIES
 *************************************/
#include <stdint.h>
#include <string.h>
#include "stm32f401re.h"

/**************************************
 * MACROS
 *************************************/

/* see the GPIO_PIN_MODES reference, this is cool */
/*** PIN MODES @GPIO_PIN_MODES ***/
#define GPIO_MODE_IN		0x00
#define GPIO_MODE_OUT		0x01
#define GPIO_MODE_ALT		0x02
#define GPIO_MODE_ANALOG	0x03
/* INTERRUPT, falling, rising, both*/
#define GPIO_MODE_IT_FT		0x04
#define GPIO_MODE_IT_RT		0x05
#define GPIO_MODE_IT_RFT	0x06

/*** OUTPUT TYPES ***/
#define GPIO_OP_TYPE_PP		0x00
#define GPIO_OP_TYPE_OD		0x01

/*** SPEED MODES @GPIO_SPEED ***/
#define GPIO_SPEED_LOW		0x00
#define GPIO_SPEED_MED		0x01
#define GPIO_SPEED_FAST		0x02
#define GPIO_SPEED_HIGH		0x03

/*** PULLUP / PULLDOWN ***/
#define GPIO_NO_PULL		0x00
#define GPIO_PIN_PU			0x01
#define GPIO_PIN_PD			0x02

/*** GPIO PIN NUMBERS ***/
#define GPIO_PIN_NUMBER_0	0x00
#define GPIO_PIN_NUMBER_1	0x01
#define GPIO_PIN_NUMBER_2	0x02
#define GPIO_PIN_NUMBER_3	0x03
#define GPIO_PIN_NUMBER_4	0x04
#define GPIO_PIN_NUMBER_5	0x05
#define GPIO_PIN_NUMBER_6	0x06
#define GPIO_PIN_NUMBER_7	0x07
#define GPIO_PIN_NUMBER_8	0x08
#define GPIO_PIN_NUMBER_9	0x09
#define GPIO_PIN_NUMBER_10	0x0a
#define GPIO_PIN_NUMBER_11	0x0b
#define GPIO_PIN_NUMBER_12	0x0c
#define GPIO_PIN_NUMBER_13	0x0d
#define GPIO_PIN_NUMBER_14	0x0e
#define GPIO_PIN_NUMBER_15	0x0f

/**************************************
 * TYPES
 *************************************/

/* configuration for a GPIO pin */
typedef struct {
	uint8_t GPIO_pin_number;
	uint8_t GPIO_pin_mode;			/* Possible values: @GPIO_PIN_MODES */
	uint8_t GPIO_pin_speed;			/* Possible values: @GPIO_SPEED */
	uint8_t GPIO_pin_pupd_control;
	uint8_t GPIO_pin_op_type;
	uint8_t GPIO_alt_fun_mode;
} gpio_pin_config_t;

/* Handle Structure */
typedef struct {
	/* pointer to base address of GPIO peripheral */
	gpio_regdef_t *pGPIOx;				/* pointer to base of GPIOx peripheral */
	gpio_pin_config_t gpio_pin_config;	/* holds GPIO pin configuration */
} gpio_handle_t;

/**************************************
 * FUNCTION PROTOTYPES
 *************************************/

/**
 * @brief returns an empty handle for GPIO
 * @param void
 * @return a gpio handle initialized to zero
 */
gpio_handle_t GPIO_get_handle(void);

/* init, de-init */
extern void GPIO_init(gpio_handle_t *pGPIO_handle);
/* IMPORTANT: send register back to reset state with this, see RCC peripheral reset register! */
extern void GPIO_deinit(gpio_regdef_t *pGPIOx);

/**
 * 	@brief enable clock for given peripheral base address
 * 	@param pGPIOx struct holding regdef
 *	@param EnorDi enable or disable
 * 	@return void
 * */
extern void GPIO_pclk_control(gpio_regdef_t *pGPIOx, uint8_t EnorDi);

/* I/O */
extern uint8_t GPIO_read_input_pin(gpio_regdef_t *pGPIOx, uint8_t pin_number);
extern uint16_t GPIO_read_input_port(gpio_regdef_t *pGPIOx);
extern void GPIO_write_output_pin(gpio_regdef_t *pGPIOx, uint8_t pin_number, uint8_t value);
extern void GPIO_write_output_port(gpio_regdef_t *pGPIOx, uint16_t value);
extern void GPIO_toggle_output_pin(gpio_regdef_t *pGPIOx, uint8_t pin_number);

/* interrupts */
extern void GPIO_IRQ_config(uint8_t IRQ_number, uint8_t EnorDi);
extern void GPIO_IRQ_priority_cfg(uint8_t IRQ_number, uint32_t IRQ_priority);
extern void GPIO_IRQ_handling(uint8_t pin_number);	/* called by user to process interrupt */

/**************************************
 * EOF
 *************************************/
#endif /* INC_GPIO_H_ */
