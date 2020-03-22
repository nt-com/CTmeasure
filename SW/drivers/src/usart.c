#include "usart.h"

static uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
static uint8_t APB1_PreScaler[4] = { 2, 4 , 8, 16};

static uint32_t RCC_GetPCLK2Value(void) {
	uint32_t SystemClock=0,tmp,pclk2;
	uint8_t clk_src = ( RCC->CFGR >> 2) & 0X3;

	uint8_t ahbp,apb2p;

	if(clk_src == 0) {
		SystemClock = 16000000;
	} else {
		SystemClock = 8000000;
	}

	tmp = (RCC->CFGR >> 4 ) & 0xF;

	if(tmp < 0x08) {
		ahbp = 1;
	} else {
       ahbp = AHB_PreScaler[tmp-8];
	}

	tmp = (RCC->CFGR >> 13 ) & 0x7;
	if(tmp < 0x04) {
		apb2p = 1;
	} else {
		apb2p = APB1_PreScaler[tmp-4];
	}

	pclk2 = (SystemClock / ahbp )/ apb2p;

	return pclk2;
}

void USART_set_baudrate(USART_regdef_t *p_USARTx, uint32_t baudrate) {

	uint32_t PCLKx;

	uint32_t usartdiv;

	uint32_t M_part,F_part;

	uint32_t tempreg = 0;

  if(p_USARTx == USART1 || p_USARTx == USART6) {
	   /* USART1 and USART6 on APB2 bus */
	   PCLKx = RCC_GetPCLK2Value();
  } else {
	   //PCLKx = RCC_GetPCLK1Value();
  }

  if(p_USARTx->CR1 & (1 << USART_CR1_OVER8)) {
	   usartdiv = ((25 * PCLKx) / (2 *baudrate));
  } else {
	   usartdiv = ((25 * PCLKx) / (4 *baudrate));
  }

  M_part = usartdiv/100;

  tempreg |= M_part << 4;

  F_part = (usartdiv - (M_part * 100));

  if(p_USARTx->CR1 & ( 1 << USART_CR1_OVER8)) {
	  F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);
   } else {
	   F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);
   }

  tempreg |= F_part;

  p_USARTx->BRR = tempreg;
}

void USART_init(USART_Handle_t *p_usart_handle) {

	uint32_t tempreg=0;


	 USART_peripheral_clock_control(p_usart_handle->p_USARTx,ENABLE);

	if ( p_usart_handle->USART_config.mode == USART_MODE_ONLY_RX) {
		tempreg|= (1 << USART_CR1_RE);
	} else if (p_usart_handle->USART_config.mode == USART_MODE_ONLY_TX) {
		tempreg |= ( 1 << USART_CR1_TE );

	}else if (p_usart_handle->USART_config.mode == USART_MODE_TXRX) {
		tempreg |= ( ( 1 << USART_CR1_RE) | ( 1 << USART_CR1_TE) );
	}

	tempreg |= p_usart_handle->USART_config.USART_wordlength << USART_CR1_M ;


	if ( p_usart_handle->USART_config.USART_parity == USART_PARITY_EN_EVEN) {
		tempreg |= ( 1 << USART_CR1_PCE);


	} else if (p_usart_handle->USART_config.USART_parity == USART_PARITY_EN_ODD ) {
	    tempreg |= ( 1 << USART_CR1_PCE);
	    tempreg |= ( 1 << USART_CR1_PS);
	}

	p_usart_handle->p_USARTx->CR1 = tempreg;

	tempreg=0;
	tempreg |= p_usart_handle->USART_config.USART_no_stop << USART_CR2_STOP;
	p_usart_handle->p_USARTx->CR2 = tempreg;

	tempreg=0;

	if ( p_usart_handle->USART_config.USART_hw_flowcontrol == USART_HW_FLOW_CTRL_CTS) {
		tempreg |= ( 1 << USART_CR3_CTSE);


	} else if (p_usart_handle->USART_config.USART_hw_flowcontrol == USART_HW_FLOW_CTRL_RTS) {
		tempreg |= ( 1 << USART_CR3_RTSE);

	} else if (p_usart_handle->USART_config.USART_hw_flowcontrol == USART_HW_FLOW_CTRL_CTS_RTS) {
		tempreg |= ( 1 << USART_CR3_CTSE);
		tempreg |= ( 1 << USART_CR3_RTSE);
	}


	p_usart_handle->p_USARTx->CR3 = tempreg;

	USART_set_baudrate(p_usart_handle->p_USARTx,p_usart_handle->USART_config.baudrate);

}

void USART_peripheral_control(USART_regdef_t *p_USARTx, uint8_t Cmd) {
	if(Cmd == ENABLE) {
		p_USARTx->CR1 |= (1 << 13);
	} else {
		p_USARTx->CR1 &= ~(1 << 13);
	}

}

void USART_peripheral_clock_control(USART_regdef_t *p_USARTx, uint8_t set) {
	if(set == ENABLE) {
		if(p_USARTx == USART1) {
			USART1_PCCK_EN();
		} else if (p_USARTx == USART2) {
			USART2_PCCK_EN();
		} else if (p_USARTx == USART3) {
			USART3_PCCK_EN();
		}
		else if (p_USARTx == UART4) {
			UART4_PCCK_EN();
		}
	}

}

uint8_t USART_get_flag_status(USART_regdef_t *p_USARTx, uint8_t flag_name) {
    if(p_USARTx->SR & flag_name) {
    	return SET;
    }

   return RESET;
}

void USART_send_data(USART_Handle_t *p_usart_handle, uint8_t *p_tx_buffer, uint32_t len) {

	for(uint32_t i = 0 ; i < len; i++) {
		while(! USART_get_flag_status(p_usart_handle->p_USARTx,USART_FLAG_TXE)) {};

		p_usart_handle->p_USARTx->DR = ((uint8_t)(*p_tx_buffer) & (uint8_t)0xFF);
		p_tx_buffer++;

	}

	while( ! USART_get_flag_status(p_usart_handle->p_USARTx,USART_FLAG_TC));
}

void USART_clear_flag(USART_regdef_t *p_USARTx, uint16_t flag_name) {
	p_USARTx->SR &= ~( flag_name);

}
