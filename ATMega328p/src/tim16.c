/*********************************************************************
 * 16 bit Timer - C File
 * Short Name: tim16
 * Author: nmt @ NT-COM
 * Date: 23.03.2020
 * Description: functions for the 16 bit timer
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [23.03.2020][nmt]: initial commit
 *********************************************************************/

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include "tim16.h"

/*********************************************************************
 * GLOBALS
 *********************************************************************/
static uint8_t g_tim_state = TIMER_SUCCESS;

/*********************************************************************
 * INTERNAL FUNCTION PROTOTYPES
 *********************************************************************/
static void tim16_set_prescaler(uint16_t prescaler_value);

/*********************************************************************
 * INTERNAL FUNCTIONS
 *********************************************************************/
static void tim16_set_prescaler(uint16_t prescaler_value) {

	switch(prescaler_value) {

		/* see datasheet table 15-6 for this */

		case TIM16_STOP_TIMER:
			TCCR1B = 0xf8;	/* clear CS10 to CS 12 */
			break;
		case TIM16_PRE_1:
			TCCR1B |= (1 << CS10);
			break;
		case TIM16_PRE_8:
			TCCR1B |= (1 << CS11);
			break;
		case TIM16_PRE_64:
			TCCR1B |= ( (1 << CS10) | (1 << CS11) );
			break;
		case TIM16_PRE_256:
			TCCR1B |= (1 << CS12);
			break;
		case TIM16_PRE_1024:
			TCCR1B |= ( (1 << CS12) | (1 << CS10) );
			break;
		default:
			g_tim_state = TIMER_FAILURE;
			break;

	} /* switch */

}

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
extern uint8_t tim16_init(tim16_handle_t *p_tim16_handle) {

	g_tim_state = TIMER_SUCCESS;

	/* timer setup */

	/* NOTE: check out datasheet section 14 for more details */

	/* NOTE:	This is the "Clear Timer on Compare Match (CTC) Mode" 
						basically you set a value in OCR0A and the timer value 
						is continuously compared to this value. Once it matches
						the counter of the timer is cleared. When this happens
						you can trigger an interrupt, which is exactly what we
						are setting up below. To change what happens when an 
						interrupt occurs add your code to the ISR above. */

	/* this value is calculated as follows: 
		 f_OCnA = CPU_CLOCK_FREQ / ( 2*PRESCALE_VALUE*(1+OCRnA) ) */	
	OCR1A = p_tim16_handle->compare_value;

	/*	this register is used with register TCCR1A to set counting sequence,
			the maximum value for the counter and wave form generation. In this 
			case, we are setting up CTC mode and checking OCR1A if we have a 
			match. If you look in the datasheet table 15-5 describes this. */
	TCCR1B |= (1 << WGM12);	

	/* activate interrupt on compare match */
	TIMSK1 |= (1 << OCIE1A);

	/* set the prescaler */
	tim16_set_prescaler(p_tim16_handle->prescaler_value);

	return g_tim_state;

}

/*********************************************************************
 * EOF
 *********************************************************************/
