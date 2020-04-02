/*********************************************************************
 * Twin Wire Interface (TWI) Driver - C File
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
 
 /*********************************************************************
 * NOTES: 	references to the registers used are given according to the
 *					ATMega328p datasheet
 * FUTURE WORK:
 * 			add a header configuration file for SCL setup based on the
 * 			CPU frequency, add a macro for the mask used in twi_status
 *********************************************************************/

/*********************************************************************
 * LIBRARIES
 *********************************************************************/ 
#include "twi.h"

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
void twi_init(void) {
	
    /* clocking setup for SCL, calculated as follows: 
     * SCL_frequency = CPU frequency / (16+2*TWBR * Prescaler Value) 
     * For a 16 MHz clock this equates to 16 MHz / 16+2*72*1 = 100 kHz
     * using the setup below */
     
    /* determines the prescaler value for the frequency of the bus */
    TWSR = 0x00;
    /* defines the SCL period */
    TWBR = 0x48;	/* use no prescaler, thus 72 is the value needed for 100kHz */
    
    /* enables the twi module in the TWI control register */
    TWCR = (1<<TWEN);
}

void twi_start(void) {
	
	/* clear interrupt, set start condition bit for operation as master
	 *  and enable */
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    /* wait until the start condition is sent */
    while ((TWCR & (1<<TWINT)) == 0);
}

void twi_stop(void) {
	/* clear interrupt, set stop condition bit for operation as master
	 *  and enable */
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void twi_write(uint8_t ui8_data) {
		/* load the data to send in the twi data register */
    TWDR = ui8_data;
    /* clear the interrupt and enable */
    TWCR = (1<<TWINT) | (1<<TWEN);
    /* wait until the data is sent */
    while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t twi_read_ack(void) {
		/* clear the interrupt, enable and return an ACK upon reception */
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    /* wait until receiving is done */
    while ((TWCR & (1<<TWINT)) == 0);
    /* return the received data stored in the twi data register */
    return TWDR;
}

uint8_t twi_read_nack(void) {
	/* clear the interrupt and enable 
	 * this sends a NACK upon reception because TWEA is not set */
    TWCR = (1<<TWINT) | (1<<TWEN);
    /* wait until the data is received */
    while ((TWCR & (1<<TWINT)) == 0);
    /* return the received data */
    return TWDR;
}

uint8_t twi_status(void) {
	
	/* read the status register of the twi on the ATMega328p 
	 * this is useful for debugging and determining the correct 
	 * control flow of the software. For status codes refer to the 
	 * ATMega328p datasheet section 21.7.2 or util/twi.h */
	 
	/* Note that TWSR also contains the prescaler bits used in twi_init
	 * so to read the status we must mask the register. We are reading 
	 * a 5 bit status stored in the higher 5 bits,
	 * so mask with 11111000 = 0xF8 */
	
    uint8_t status = 0x00;
    status = TWSR & 0xF8;
    return status;
}

/*********************************************************************
 * EOF
 *********************************************************************/
