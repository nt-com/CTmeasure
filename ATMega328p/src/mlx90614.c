/*********************************************************************
 * MLX90614 Module - C File
 * Short Name: mlx
 * Author: nmt @ NT-COM
 * Date: 23.03.2020
 * Description: functions to interface with the sensor
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [23.03.2020][nmt]: initial commit
 *********************************************************************/

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include "mlx90614.h"

/*********************************************************************
 * GLOBALS
 *********************************************************************/
/* necessary delimiter for UART */
static uint8_t g_message_buffer[3] = { 0x00 };
static uint8_t g_error_code = 0x00;

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
extern uint8_t mlx_read(uint8_t *temperature) {

	g_error_code = OK;

	twi_start();
	if(twi_status() != TW_START) {
		g_error_code = FAILURE_TW_START;
		goto stop;
	}	
	
	twi_write(MLX_I2C_ADDR);
	if(twi_status() != TW_MT_SLA_ACK) {
		g_error_code = FAILURE_TW_MT_SLA_ACK;
		goto stop;
	}
	
	twi_write(TEMPERATURE_REGISTER_ADDR);
	if(twi_status() != TW_MT_DATA_ACK) {
		g_error_code = FAILURE_TW_MT_DATA_ACK;
		goto stop;
	}

	/* repeated start condition */
	twi_start();
	if(twi_status() != TW_REP_START) {
		g_error_code = FAILURE_TW_REP_START;
		goto stop;
	}
	
	/* read operation */
	twi_write((MLX_I2C_ADDR) | 1);
	if(twi_status() != TW_MR_SLA_ACK) {
		g_error_code = FAILURE_TW_MR_SLA_ACK;
		goto stop;
	}
	
	/* read temperature data, last byte is CRC */
	/* get data and tell sensor we want no more */
	*(g_message_buffer+0) = twi_read_ack();
	if(twi_status() != TW_MR_DATA_ACK) {
		g_error_code = FAILURE_TW_MR_DATA_ACK;

	}
	*(g_message_buffer+1) = twi_read_ack();
	if(twi_status() != TW_MR_DATA_ACK) {
		g_error_code = FAILURE_TW_MR_DATA_ACK;

	}
	/* get last data byte and tell sensor we want no more */
	*(g_message_buffer+2) = twi_read_nack();
	if(twi_status() != TW_MR_DATA_NACK) {
		g_error_code = FAILURE_TW_MR_DATA_NACK;
	}

stop:

	/* stop condition to sensor */
	twi_stop();

	*(temperature+1) = g_message_buffer[0];
	*(temperature+0) = g_message_buffer[1];

	return g_error_code;

}


/*********************************************************************
 * EOF
 *********************************************************************/
