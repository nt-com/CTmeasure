/*
 * gpio.c
 *
 *  Created on: Feb 5, 2020
 *      Author: nmt
 */

/**************************************
 * LIBRARIES
 *************************************/
#include "gpio.h"

/**************************************
 * FUNCTIONS
 *************************************/

gpio_handle_t GPIO_get_handle(void) {
	gpio_handle_t handle;
	memset(&handle, 0, sizeof(gpio_handle_t));
	return handle;
}

void GPIO_init(gpio_handle_t *pGPIO_handle) {

	uint32_t temp = 0x00;
	uint32_t offset = 0x00;	/* indexing for high and low registers */

	GPIO_pclk_control(pGPIO_handle->pGPIOx, ENABLE);

	/* before setting bitfield, clear them */

	/* configure mode, output or input, speed, etc. */
	/* mode - non interrupt and interrupt modes */
	if(pGPIO_handle->gpio_pin_config.GPIO_pin_mode <= GPIO_MODE_ANALOG) {
		/* get user values from handle */
		temp = (pGPIO_handle->gpio_pin_config.GPIO_pin_mode << ( 2 * pGPIO_handle->gpio_pin_config.GPIO_pin_number));
		/* write register */
		pGPIO_handle->pGPIOx->MODER &= ~(0x03 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);	/* IMPORTANT: clear before write */
		pGPIO_handle->pGPIOx->MODER |= temp;	/* IMPORTANT: never equate, will reinit the whole register */
		/* reset */
		temp = 0x00;
	} else {

		/* interrupt modes */
		/* (1) select trigger */
		if(pGPIO_handle->gpio_pin_config.GPIO_pin_mode == GPIO_MODE_IT_FT) {
			/* clear the corresponding rising edge trigger bit */
			EXTI->RTSR &= ~(1 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);
			/* set falling edge trigger */
			EXTI->FTSR |= (1 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);
		} else if(pGPIO_handle->gpio_pin_config.GPIO_pin_mode == GPIO_MODE_IT_RT) {
			EXTI->FTSR &= ~(1 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);
			/* set falling edge trigger */
			EXTI->RTSR |= (1 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);
		} else if(pGPIO_handle->gpio_pin_config.GPIO_pin_mode == GPIO_MODE_IT_RFT) {
			EXTI->RTSR |= (1 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);
			/* set falling edge trigger */
			EXTI->FTSR |= (1 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);

		}

		/* (2) configure GPIO port selection SYSCFG_EXTICR */
		/* Register: [Pin 12-15][Pin 8-11][Pin 4-7][Pin 0-3]
		 * Port Assignment: 0000 = PAx, 0001 = PBx, ...
		 * Example PD3 -> use EXTICR[0], assign 0x03
		 * Example PC13 -> use EXTICR[3] assign 0x02
		 * */
		/* get array index */
		uint8_t index = pGPIO_handle->gpio_pin_config.GPIO_pin_number / 4;
		/* see datasheet for offset calculation, reduce pin number, move by 4 */
		offset = (pGPIO_handle->gpio_pin_config.GPIO_pin_number % 4);
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIO_handle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[index] = ( portcode << (offset * 4));

		/* (3) enable EXTI interrupt delivery using IMR (interrupt mask register) */
		EXTI->IMR |= (1 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);
	}

	/* speed */
	temp = (pGPIO_handle->gpio_pin_config.GPIO_pin_speed << ( 2 * pGPIO_handle->gpio_pin_config.GPIO_pin_number ));

	pGPIO_handle->pGPIOx->OSPEEDR &= ~(0x03 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);	/* IMPORTANT: clear before write */
	pGPIO_handle->pGPIOx->OSPEEDR |= temp;
	temp = 0;

	/* pupd */
	temp = (pGPIO_handle->gpio_pin_config.GPIO_pin_pupd_control << ( 2 * pGPIO_handle->gpio_pin_config.GPIO_pin_number ));
	pGPIO_handle->pGPIOx->PUPDR &= ~(0x03 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);	/* IMPORTANT: clear before write */
	pGPIO_handle->pGPIOx->PUPDR |= temp;
	temp = 0;

	/* output type */
	temp = (pGPIO_handle->gpio_pin_config.GPIO_pin_op_type << ( pGPIO_handle->gpio_pin_config.GPIO_pin_number ));
	pGPIO_handle->pGPIOx->OTYPER &= ~(0x01 << pGPIO_handle->gpio_pin_config.GPIO_pin_number);	/* IMPORTANT: clear before write */
	pGPIO_handle->pGPIOx->OTYPER |= temp;
	temp = 0;

	/* alternate functionality */
	if(pGPIO_handle->gpio_pin_config.GPIO_pin_mode == GPIO_MODE_ALT) {
		/* only if alternate function is requested */
		temp = pGPIO_handle->gpio_pin_config.GPIO_pin_number / 8;
		offset = pGPIO_handle->gpio_pin_config.GPIO_pin_number % 8;

		pGPIO_handle->pGPIOx->AFR[temp] &= ~(0x0f << (4 * offset));	/* IMPORTANT: clear before write */
		pGPIO_handle->pGPIOx->AFR[temp] |= (pGPIO_handle->gpio_pin_config.GPIO_alt_fun_mode << (4 * offset));
	}

}

void GPIO_deinit(gpio_regdef_t *pGPIOx) {

	/* reset all registers of the GPIO port using the RCC reset register RCC_RSTR registers */
		if(pGPIOx == GPIOA) {
			GPIOA_REG_RESET();
		}
		else if(pGPIOx == GPIOB) {
			GPIOB_REG_RESET();
		}
		else if(pGPIOx == GPIOB) {
			GPIOB_REG_RESET();
		}
		else if(pGPIOx == GPIOC) {
			GPIOC_REG_RESET();
		}
		else if(pGPIOx == GPIOD) {
			GPIOD_REG_RESET();
		}
		else if(pGPIOx == GPIOE) {
			GPIOE_REG_RESET();
		}
		else if(pGPIOx == GPIOH) {
			GPIOH_REG_RESET();
		}



}

/**	IMPORTANT: this is a good structure for comments
 * @brief configure peripheral clock, enables or disables clock
 * @param pGPIOx base address of the GPIO peripheral
 * @param EnorDi enable or disable [ENABLE, DISABLE]
 * @return void
 * @note -
 */
void GPIO_pclk_control(gpio_regdef_t *pGPIOx, uint8_t EnorDi) {

	if(EnorDi == ENABLE) {

		if(pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx == GPIOH) {
			GPIOH_PCLK_EN();
		}

	} else {

		if(pGPIOx == GPIOA) {
			GPIOA_PCLK_DI();
		}
		else if(pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx == GPIOC) {
			GPIOC_PCLK_DI();
		}
		else if(pGPIOx == GPIOD) {
			GPIOD_PCLK_DI();
		}
		else if(pGPIOx == GPIOE) {
			GPIOE_PCLK_DI();
		}
		else if(pGPIOx == GPIOH) {
			GPIOH_PCLK_DI();
		}

	}

}

uint8_t GPIO_read_input_pin(gpio_regdef_t *pGPIOx, uint8_t pin_number) {

	uint8_t value = 0x00;
	value = (uint8_t)((pGPIOx->IDR >> pin_number) & 0x00000001);
	return value;

}

uint16_t GPIO_read_input_port(gpio_regdef_t *pGPIOx) {

	/* alternative: return pGPIO->IDR */
	uint16_t value = 0x00;
	value = (uint16_t)(pGPIOx->IDR);
	return value;

}

void GPIO_write_output_pin(gpio_regdef_t *pGPIOx, uint8_t pin_number, uint8_t value) {

	if(value == GPIO_PIN_SET) {
		pGPIOx->ODR	|= (1 << pin_number);
	} else {
		pGPIOx->ODR	&= ~(1 << pin_number);
	}

}

void GPIO_write_output_port(gpio_regdef_t *pGPIOx, uint16_t value) {
	pGPIOx->ODR = value;

}

void GPIO_toggle_output_pin(gpio_regdef_t *pGPIOx, uint8_t pin_number) {
	pGPIOx->ODR ^= (1 << pin_number);
}

void GPIO_IRQ_config(uint8_t IRQ_number, uint8_t EnorDi) {

	/* interrupt configuration in NVIC (processor side configuration) */

	/*** ENABLE / DISABLE INTERRUPT ***/
	/* see cortex m4 generic user guide for NVIC registers */
	/* NVIC ISER0 enables interrupts 0-31, ISER1 enables interrupts 32-63, ... */
	/* NVIC ICER0 disables interrupts 0-31, ISER1 disables interrupts 32-63, ... */
	if(EnorDi == ENABLE) {
		if(IRQ_number <= 31) { /* ISER0 */
			*NVIC_ISER0 |= (1 << IRQ_number);
		} else if(IRQ_number > 31 && IRQ_number < 64) {
			*NVIC_ISER1 |= (1 << (IRQ_number % 32) );
		} else if(IRQ_number >= 64 && IRQ_number < 96) {
			*NVIC_ISER2 |= (1 << (IRQ_number % 64) );
		}

	} else { /* disable */
		if(IRQ_number <= 31) { /* ISER0 */
			*NVIC_ICER0 |= (1 << IRQ_number);
		} else if(IRQ_number > 31 && IRQ_number < 64) {
			*NVIC_ICER1 |= (1 << (IRQ_number % 32) );
		} else if(IRQ_number >= 64 && IRQ_number < 96) {
			*NVIC_ICER2 |= (1 << (IRQ_number % 64) );
		}
	} /* end enable/disable interrupt */

}

void GPIO_IRQ_priority_cfg(uint8_t IRQ_number, uint32_t IRQ_priority) {

	/*** INTERRUPT PRIORITY ***/
	/* see cortex m4 generic user guide, you can set the same priority level for 4 interrupt numbers
	 * IRQ number 236: find register 236/4	=  59, find section 236 % 4 = 0
	 * */

	uint8_t iprx = IRQ_number / 4; /* register */
	uint8_t iprx_section = IRQ_number % 4; /* section in register */
	uint8_t shift_amount = ( 8 * iprx_section ) + ( 8 - NO_PR_BITS_IMPLEMENTED );

	/* jump to the right register IPR0 = 0x400, IPR1 = 0x404, ...
	 * now you have 4 sections of 8 bits, so shift 8*section
	 * only higher 4 bits implemented in each section, so add 4
	 * */
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQ_priority << shift_amount);

}

void GPIO_IRQ_handling(uint8_t pin_number) {

	/* interrupt pending? clear it! */
	if(EXTI->PR & (1 << pin_number)) {
		/* writing one clears PR register */
		EXTI->PR |= (1 << pin_number);
	}

}


