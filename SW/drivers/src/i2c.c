/*
 * i2c.c
 *
 *  Created on: Feb 29, 2020
 *      Author: nmt
 */

/***********************************************
 * LIBRARIES
 */
#include "i2c.h"

/***********************************************
 * MACROS
 */

/***********************************************
 * GLOBALS
 */
/* clock division factors for I2C speed setup */
const uint16_t AHB_DIV_FACTORS[9] = { 2, 4, 8, 16, 32, 64, 128, 256, 512 };
const uint16_t APB1_DIV_FACTORS[4] = { 2, 4, 8, 16 };

/***********************************************
 * INTERNAL FUNCTION PROTOTYPES
 */

/* @brief obtain RCC clock value for I2C speed setup */
static uint32_t RCC_get_pclk1(void);

/* @brief activate RCC I2C clock */
static void I2C_peripheral_clock_on(i2c_regdef_t *pI2Cx);

/* @brief generate I2C start condition */
static void I2C_gen_start(i2c_regdef_t *pI2Cx);
/* @brief generate I2C stop condition */
static void I2C_gen_stop(i2c_regdef_t *pI2Cx);

/* @brief executes I2C address phase for a given slave address, either in read or write mode */
static void I2C_exec_addr_phase(i2c_regdef_t *pI2Cx, uint8_t slave_address, uint8_t rw);
/* @brief clear ADDR flag after address phase is complete */
static void I2C_clear_addr_flag(i2c_regdef_t *pI2Cx);

/* @brief activates the clock in RCC for a selected I2C peripheral */
void I2C_peripheral_clock_on(i2c_regdef_t *pI2Cx);

/***********************************************
 * PRIVATE FUNCTIONS
 */
static uint32_t RCC_get_pclk1(void) {

	uint32_t system_clock = 0;
	uint32_t pclk1 = 0;
	uint8_t clksrc;
	uint8_t AHB_prescaler_value = 0;
	uint8_t APB1_prescaler_value = 0;

	clksrc = (RCC->CFGR >> 2) & 0x03;

	if(clksrc == 0) {
		/* HSI */
		system_clock = 16000000;
	} else if(clksrc == 1) {
		/* HSE */
		system_clock = 8000000;
	} else if(clksrc == 2) {
		/* PLL */
		//system_clock = RCC_GetPLLOutputClock();
	}

	/* get AHB prescaler value */
	AHB_prescaler_value = (RCC->CFGR >> 4) & 0x0f;

	if(AHB_prescaler_value < 8) {
		AHB_prescaler_value = 1;
	} else {
		AHB_prescaler_value = AHB_DIV_FACTORS[AHB_prescaler_value - 8];
	}

	/* get AP1 prescaler */
	APB1_prescaler_value = (RCC->CFGR >> 10) & 0x7;

	if(APB1_prescaler_value < 4) {
		APB1_prescaler_value = 1;
	} else {
		APB1_prescaler_value = APB1_DIV_FACTORS[APB1_prescaler_value - 4];
	}

	pclk1 = (system_clock / AHB_prescaler_value) / APB1_prescaler_value;

	return pclk1;

}

static void I2C_gen_start(i2c_regdef_t *pI2Cx) {
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

static void I2C_gen_stop(i2c_regdef_t *pI2Cx) {
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

static void I2C_exec_addr_phase(i2c_regdef_t *pI2Cx, uint8_t slave_address, uint8_t rw) {

	slave_address = slave_address << 1;
	if(rw == I2C_WRITE) {
		slave_address &= ~(1);	/* set write bit */
	} else if(rw == I2C_READ) {
		slave_address |= 1;	/* set read bit */
	}
	pI2Cx->DR = slave_address;

}

static void I2C_clear_addr_flag(i2c_regdef_t *pI2Cx) {

	uint32_t dummyread = pI2Cx->SR1;
	dummyread = pI2Cx->SR2;

	(void)dummyread;
}

static void I2C_peripheral_clock_on(i2c_regdef_t *pI2Cx) {
			if(pI2Cx == I2C1) {
				I2C1_PCLK_EN();
			} else if(pI2Cx == I2C2) {
				I2C2_PCLK_EN();
			} else if(pI2Cx == I2C2) {
				I2C3_PCLK_EN();
			}
}

/***********************************************
 * EXTERNAL FUNCTIONS
 */
void I2C_init(i2c_handle_t *p_i2c_handle) {

	uint32_t tempreg = 0;
	uint16_t ccr_value = 0;

	I2C_peripheral_clock_on(p_i2c_handle->pI2Cx);

	tempreg |= (p_i2c_handle->I2C_config.I2C_ack_control << I2C_CR1_ACK);
	p_i2c_handle->pI2Cx->CR1 |= tempreg;

	/* user may change clock frequency, we must calculate it */
	tempreg = 0;
	tempreg |= RCC_get_pclk1() / 1000000U;	/* returns PCLK in Hz, want MHz */
	p_i2c_handle->pI2Cx->CR2 = tempreg & 0x3f;

	tempreg = 0;
	/* address settings, set slave address */
	tempreg |= (p_i2c_handle->I2C_config.I2C_device_address << 1);
	/* 14 must always be 1, but is marked as reserved !? */
	tempreg |= (1 << 14);
	p_i2c_handle->pI2Cx->OAR1 = tempreg;

	/* configure clock control */
	tempreg = 0;

	if(p_i2c_handle->I2C_config.I2C_SCL_speed <= I2C_SCL_SPEED_SM) {
		/* standard mode */
		ccr_value = RCC_get_pclk1() / (2 * p_i2c_handle->I2C_config.I2C_SCL_speed);
		tempreg |= ccr_value & 0xfff;
	} else {
		/* fast mode */
		tempreg |= (1 << I2C_CCR_FS);
		tempreg |= (p_i2c_handle->I2C_config.I2C_fm_duty_cycle << I2C_CCR_DUTY);

		if(p_i2c_handle->I2C_config.I2C_fm_duty_cycle == I2C_FM_DUTY_2) {
			ccr_value = RCC_get_pclk1() / (3 * p_i2c_handle->I2C_config.I2C_SCL_speed);
		} else {
			ccr_value = RCC_get_pclk1() / (25 * p_i2c_handle->I2C_config.I2C_SCL_speed);
		}

		tempreg |= (uint32_t)ccr_value & 0xfff;

	}

	p_i2c_handle->pI2Cx->CCR = tempreg;

	/* trise register configuration */
	if(p_i2c_handle->I2C_config.I2C_SCL_speed <= I2C_SCL_SPEED_SM) {
		/* standard mode */
		/* 1000 ns max rise time, thus divide PCLK1 frequency by 1 MHz, add 1 (refman) */
		tempreg = (RCC_get_pclk1() / 1000000U) + 1;
	} else {
		/* fast mode */
		tempreg = (RCC_get_pclk1() * 300) / 1000000000U + 1;
	}

	p_i2c_handle->pI2Cx->TRISE = (tempreg & 0x3f);


}

void I2C_deinit(i2c_regdef_t *pI2Cx) {

	if(pI2Cx == I2C1) {
		I2C1_REG_RESET();
	} else if(pI2Cx == I2C2) {
		I2C2_REG_RESET();
	} else if(pI2Cx == I2C2) {
		I2C3_REG_RESET();
	}

}

void i2c_master_send(i2c_handle_t *p_i2c_handle, uint8_t *p_tx_buffer, uint32_t len, uint8_t slave_address, uint8_t Sr) {

	/* generate start condition */
	I2C_gen_start(p_i2c_handle->pI2Cx);

	/* wait until SB is set, to indicate start condition was sent
	 * to reset: read SR1, which we are doing below and write address of slave to DR */
	while( ! I2C_flag_status(p_i2c_handle->pI2Cx, I2C_FLAG_SB));

	/* send address, RW bit to zero */
	I2C_exec_addr_phase(p_i2c_handle->pI2Cx, slave_address, I2C_WRITE);

	/* confirm address phase is completed, ADDR flag */
	while( ! I2C_flag_status(p_i2c_handle->pI2Cx, I2C_FLAG_ADDR));
	/* clear addr flag */
	I2C_clear_addr_flag(p_i2c_handle->pI2Cx);

	/* send data until length is zero */
	while(len > 0) {

		while( ! I2C_flag_status(p_i2c_handle->pI2Cx, I2C_FLAG_TXE));
		p_i2c_handle->pI2Cx->DR = *(p_tx_buffer);
		p_tx_buffer++;
		len--;

	}

	/* close communication when done */
	while( ! I2C_flag_status(p_i2c_handle->pI2Cx, I2C_FLAG_TXE));
	while( ! I2C_flag_status(p_i2c_handle->pI2Cx, I2C_FLAG_BTF));

	/* generate stop condition */
	if(Sr == I2C_DISABLE_SR) {
		I2C_gen_stop(p_i2c_handle->pI2Cx);
	}
}

void I2C_master_recv(i2c_handle_t *p_i2c_handle, uint8_t *p_rx_buffer, uint32_t len, uint8_t slave_address, uint8_t Sr) {

	/* (1) Generate Start Condition */
	I2C_gen_start(p_i2c_handle->pI2Cx);

	/* (2) 	Check SB flag, clear when 1 (we are already clearing below
	 * 		as I2C_flag_status reads from SR1) */
	while(! (I2C_flag_status(p_i2c_handle->pI2Cx, I2C_FLAG_SB)));

	/* (3) Send Slave address + RW bit set to 1 */
	I2C_exec_addr_phase(p_i2c_handle->pI2Cx, slave_address, I2C_READ);

	/* (4) check for ACK using the ADDR flag, if received clear ADDR */
	while(! (I2C_flag_status(p_i2c_handle->pI2Cx, I2C_FLAG_ADDR)));

	/* (5) wait until length is one */

	if(len == 1) {

		/* disable ack'ing */
		I2C_ACK(p_i2c_handle->pI2Cx, DISABLE);
		/* clear the ADDR flag */
		I2C_clear_addr_flag(p_i2c_handle->pI2Cx);
		/* generate stop condition */
		//I2C_gen_stop(p_i2c_handle->pI2Cx);
		/* wait until RXNE = 1 */
		while(! I2C_flag_status(p_i2c_handle->pI2Cx, I2C_FLAG_RXNE));
		///* generate stop condition */
		if(Sr == I2C_DISABLE_SR) {
			I2C_gen_stop(p_i2c_handle->pI2Cx);
		}
		/* read data to buffer */
		*(p_rx_buffer) = p_i2c_handle->pI2Cx->DR;

		/* always do stop after clear ADDR */

	} else {

		/* complete address phase */
		I2C_clear_addr_flag(p_i2c_handle->pI2Cx);

		for(uint32_t i = len; i > 0; i--) {

			/* wait until RXNE = 1 */
			while(! I2C_flag_status(p_i2c_handle->pI2Cx, I2C_FLAG_RXNE));

			if(i == 2) {
				/* disable ack'ing */
				I2C_ACK(p_i2c_handle->pI2Cx, DISABLE);
				/* generate stop condition */
				if(Sr == I2C_DISABLE_SR) {
					I2C_gen_stop(p_i2c_handle->pI2Cx);
				}
			}

			/* read data to buffer */
			*(p_rx_buffer) = p_i2c_handle->pI2Cx->DR;

			p_rx_buffer++;

		} // while len

	} // else

	/* re-enable ACK */
	if(p_i2c_handle->I2C_config.I2C_ack_control == I2C_ACK_ENABLE) {
		I2C_ACK(p_i2c_handle->pI2Cx, ENABLE);
	}

}

void I2C_ACK(i2c_regdef_t *pI2Cx, uint8_t set) {
	if(set == ENABLE) {
		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	} else {
		pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
	}
}

void I2C_peripheral_control(i2c_regdef_t *pI2Cx, uint8_t set) {

	if(set) {
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
//		if(pI2Cx == I2C1) {
//			I2C1_PCLK_EN();
//		} else if(pI2Cx == I2C2) {
//			I2C2_PCLK_EN();
//		} else if(pI2Cx == I2C2) {
//			I2C3_PCLK_EN();
//		}
	} else {
		pI2Cx->CR1 &= ( 1 << I2C_CR1_PE);
//		if(pI2Cx == I2C1) {
//			I2C1_PCLK_DI();
//		} else if(pI2Cx == I2C2) {
//			I2C2_PCLK_DI();
//		} else if(pI2Cx == I2C2) {
//			I2C3_PCLK_DI();
//		}
	}

}

uint8_t I2C_flag_status(i2c_regdef_t *pI2Cx, uint32_t flagname) {
	if( pI2Cx->SR1 & flagname ) {
		return FLAG_SET;
	} else {
		return FLAG_RESET;
	}
}

/***********************************************
 * EOF
 */
