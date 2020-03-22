/*
 * usart.h
 *
 *  Created on: Mar 21, 2020
 *      Author: nmt
 */

#ifndef INC_USART_H_
#define INC_USART_H_

/***********************************************
 * LIBRARIES
 */
#include "stm32f401re.h"

/***********************************************
 * MACROS
 */
#define USART6_ALT_FUNCTION_MODE 8

/***********************************************
 * TYPES
 */
/* @brief USART user configuration */
typedef struct {
	uint8_t mode;
	uint32_t baudrate;
	uint8_t USART_no_stop;
	uint8_t USART_wordlength;
	uint8_t USART_parity;
	uint8_t USART_hw_flowcontrol;
}USART_config_t;

/* @brief USART handle */
typedef struct {
	USART_regdef_t *p_USARTx;
	USART_config_t   USART_config;
	uint8_t *p_tx_buffer;
	uint8_t *p_rx_buffer;
	uint32_t tx_len;
	uint32_t rx_len;
	uint8_t tx_busy_state;
	uint8_t rx_busy_state;
} USART_Handle_t;

/* @brief USART modes */
#define USART_MODE_ONLY_TX 	0
#define USART_MODE_ONLY_RX 	1
#define USART_MODE_TXRX  	2

/* @brief supported USART baudrates */
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000

/* @brief parity options */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0

/* @brief possible wordlengths for USART */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

/* @brief stop bit configurations */
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3

/* @brief hardware flow control options */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

/* @brief USART status flags */
#define USART_FLAG_TXE 			( 1 << USART_SR_TXE)
#define USART_FLAG_RXNE 		( 1 << USART_SR_RXNE)
#define USART_FLAG_TC 			( 1 << USART_SR_TC)

/* @brief application states */
#define USART_BUSY_IN_RX 1
#define USART_BUSY_IN_TX 2
#define USART_READY 0

/* @brief USART events and errors */
#define 	USART_EVENT_TX_CMPLT   0
#define		USART_EVENT_RX_CMPLT   1
#define		USART_EVENT_IDLE      2
#define		USART_EVENT_CTS       3
#define		USART_EVENT_PE        4
#define		USART_ERR_FE     	5
#define		USART_ERR_NE    	 6
#define		USART_ERR_ORE    	7

/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/


/**
 * @brief sets up the peripheral clock for the USART
 * @param p_USARTx the USART peripheral
 * @param set set or reset the clock
 * @return void
 */
void USART_peripheral_clock_control(USART_regdef_t *p_USARTx, uint8_t set);

/**
 * @brief init and deinit the USART using a given handle
 * @param p_usart_handle the handle to use for init and deinit
 * @return void
 */
void USART_init(USART_Handle_t *p_usart_handle);
void USART_deinit(USART_Handle_t *p_usart_handle);

/**
 * @brief send data using the USART
 * @param p_usart_handle the handle to the USART
 * @param p_tx_buffer the buffer holding the data to send
 * @param len the length of the data in bytes to send
 * @return void
 */
void USART_send_data(USART_Handle_t *p_usart_handle, uint8_t *p_tx_buffer, uint32_t len);

/* @brief helper functions to get/set/clear flags, and peripheral control */
uint8_t USART_get_flag_status(USART_regdef_t *p_USARTx, uint8_t flag_name);
void USART_clear_flag(USART_regdef_t *p_USARTx, uint16_t flag_name);
void USART_peripheral_control(USART_regdef_t *p_USARTx, uint8_t set);
void USART_set_baudrate(USART_regdef_t *p_USARTx, uint32_t baudrate);

#endif /* INC_USART_H_ */
