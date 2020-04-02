/*********************************************************************
 * UART Interface  Driver - Configuration File
 * Short Name: uart_cfg
 * Author: nmt @ NT-COM
 * Date: 15.06.2019
 * Description: Settings for the UART 
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [15.09.2019][nmt]: initial commit
 *********************************************************************/

#ifndef UART_CFG_H
#define UART_CFG_H

/* set CPU frequency for prescaler value */
#ifndef F_CPU
	#warning "UART_CFG: F_CPU was undefined setting to 16 MHz"
	#define F_CPU 16000000UL
#endif

 /* baudrate we want to use */
#define BAUDRATE 9600
/* calculate prescaler value */
#define PRESCALE_VALUE (((F_CPU / (BAUDRATE * 16UL))) - 1)    

#endif
