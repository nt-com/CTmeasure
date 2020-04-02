/*********************************************************************
 * Twin Wire Interface (TWI) Driver - Header File
 * Short Name: twi
 * Author: nmt @ NT-COM
 * Date: 14.09.2019
 * Description: functions to interface with a TWI device on ATMega328p
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [14.09.2019][nmt]: initial commit
 *********************************************************************/

#ifndef TWI_H
#define TWI_H

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include <stdint.h>
#include <util/twi.h>

/*********************************************************************
 * TYPES
 *********************************************************************/
const enum {
	OK,
	FAILURE_TW_START,
	FAILURE_TW_MT_SLA_ACK,
	FAILURE_TW_MT_DATA_ACK,
	FAILURE_TW_REP_START,
	FAILURE_TW_MR_SLA_ACK,
	FAILURE_TW_MR_DATA_ACK,
	FAILURE_TW_MR_DATA_NACK	
} TWI_STATUS_CODES;  

/*********************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************/

/** 
 * @brief initializes the ATMega328p TWI interface
 * @return void 
 */
void twi_init(void);

/** 
 * @brief sends the start condition
 * @return void 
 */
void twi_start(void);

/** 
 * @brief sends the stop condition
 * @return void 
 */
void twi_stop(void);

/** 
 * @brief writes one byte of data to the bus
 * @param ui8_data the byte to write to the bus range: [0x00 - 0xff]
 * @return void 
 */
void twi_write(uint8_t ui8_data);

/** 
 * @brief reads one byte from the bus, sends ACK
 * NOTE: sending a ACK means we want to read more data
 * @return the received byte [0x00 - 0xff]
 */
uint8_t twi_read_ack(void);

/** 
 * @brief reads one byte from the bus, sends NACK 
 * NOTE: sending a NACK means we don't want to read any more data
 * @return the received byte range: [0x00 - 0xff]
 */
uint8_t twi_read_nack(void);

/**
 * @brief gets the status of the TWI
 * NOTE: the status codes are defined in util/twi.h use them!
 * @return the status value 
 */
uint8_t twi_status(void);

/*********************************************************************
 * EOF
 *********************************************************************/
#endif
