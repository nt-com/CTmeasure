/*
 * i2c.h
 *
 *  Created on: Feb 29, 2020
 *      Author: nmt
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

/***********************************************
 * LIBRARIES
 */
#include "stm32f401re.h"

/***********************************************
 * MACROS
 */

/* @speed */
#define	 I2C_SCL_SPEED_SM	100000	/* kHz */
#define	 I2C_SCL_SPEED_FM4K	400000  /* kHz */
#define	 I2C_SCL_SPEED_FM2K	200000	/* kHz */

/* @ackcontrol */
#define	 I2C_ACK_ENABLE		1
#define	 I2C_ACK_DISABLE		0

/* @dutycycle */
#define	 I2C_FM_DUTY_2		0
#define	 I2C_FM_DUTY_16_9	1

/* @operationcodes read or write */
#define I2C_WRITE	0
#define I2C_READ	1

/* @statusflags */
#define	 I2C_FLAG_TXE		( 1 << I2C_SR1_TXE )
#define	 I2C_FLAG_RXNE		( 1 << I2C_SR1_RXNE )
#define	 I2C_FLAG_SB		( 1 << I2C_SR1_SB )
#define	 I2C_FLAG_ADDR		( 1 << I2C_SR1_ADDR)
#define	 I2C_FLAG_BTF		( 1 << I2C_SR1_BTF )
#define	 I2C_FLAG_STOPF		( 1 << I2C_SR1_STOPF )
#define	 I2C_FLAG_BERR		( 1 << I2C_SR1_BERR )
#define	 I2C_FLAG_ARLO		( 1 << I2C_SR1_ARLO )
#define	 I2C_FLAG_AF		( 1 << I2C_SR1_AF )
#define	 I2C_FLAG_OVR		( 1 << I2C_SR1_OVR )
#define	 I2C_FLAG_TIMEOUT	( 1 << I2C_SR1_TIMEOUT )	/* clock stretching time, see I2C specs  */

#define I2C_DISABLE_SR	RESET	/* no repeated start */
#define I2C_ENABLE_SR	SET		/* repeated start */

/* @appstates */
#define	 I2C_READY			0
#define I2C_BUSY_IN_RX		1
#define I2C_BUSY_IN_TX		2

/***********************************************
 * TYPES
 */
typedef struct i2c_config {
	uint32_t I2C_SCL_speed;
	uint8_t I2C_device_address;
	uint8_t I2C_ack_control;
	uint16_t I2C_fm_duty_cycle;
} i2c_config_t;

typedef struct i2c_handle {
	i2c_regdef_t *pI2Cx;
	i2c_config_t I2C_config;
	uint8_t *p_tx_buffer;
	uint8_t *p_rx_buffer;
	uint8_t device_address;
	uint8_t Sr;
} i2c_handle_t;

/***********************************************
 * GLOBALS
 */

/***********************************************
 * PUBLIC FUNCTION PROTOTYPES
 */

/**
 * @brief initializes I2C using the configuration given by the handle
 * @param p_i2c_handle the I2C configuration handle
 * @return void
 */
void I2C_init(i2c_handle_t *p_i2c_handle);

/**
 * @brief resets the registers of a I2C peripheral
 * @param pI2Cx pointer to the I2C peripheral
 * @return void
 */
void I2C_deinit(i2c_regdef_t *pI2Cx);

/**
 * @brief used by master to transmit data
 * @param p_i2c_handle the handle for the I2C peripheral
 * @param p_tx_buffer the buffer holding the data to send
 * @param len the number of bytes in the buffer
 * @param slave_address the slave address to send to
 * @param Sr activate or deactivate repeated start
 * @return void
 */
void i2c_master_send(i2c_handle_t *p_i2c_handle, uint8_t *p_tx_buffer,
		uint32_t len, uint8_t slave_address, uint8_t Sr);

/**
 * @brief used by master to receive data
 * @param p_i2c_handle the handle for the I2C peripheral
 * @param p_rx_buffer the buffer to hold the received data
 * @param len the number of bytes in the buffer
 * @param slave_address the slave address to receive from
 * @param Sr activate or deactivate repeated start
 * @return void
 */
void I2C_master_recv(i2c_handle_t *p_i2c_handle, uint8_t *p_rx_buffer,
		uint32_t len, uint8_t slave_address, uint8_t Sr);

/**
 * @brief used by master to transmit data
 * @param pI2Cx pointer to the I2C peripheral
 * @param set activate or deactivate ACKing (for NACKs after transaction)
 * @return void
 */
void I2C_ACK(i2c_regdef_t *pI2Cx, uint8_t set);

/**
 * @brief enables the I2C peripheral
 * @param pI2Cx pointer to the I2C peripheral
 * @param set activate or deactivate
 * @return void
 */
void I2C_peripheral_control(i2c_regdef_t *pI2Cx, uint8_t set);

/**
 * @brief helper function to obtain I2C flags from the SRAM
 * @param pI2Cx pointer to the I2C peripheral
 * @param flag_name name of the flag to obtain
 * @return flag value
 */
uint8_t I2C_flag_status(i2c_regdef_t *pI2Cx, uint32_t flagname);


/***********************************************
 * EOF
 */
#endif /* INC_I2C_H_ */
