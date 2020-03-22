/*
 * app.c
 *
 *  Created on: Mar 22, 2020
 *      Author: nmt
 */

/***********************************************
 * LIBRARIES
 */
#include "app.h"


/***********************************************
 * MACROS
 */

/***********************************************
 * GLOBALS
 */

/* peripheral handles, application state */
/* @brief application state, checks if modules are initialized */
static uint8_t g_application_state = APP_NOT_INITIALIZED;
/* @brief a handle to the USART6 peripheral, see usart.h*/
static USART_Handle_t g_usart6_handle = {0};
/* @brief a handle to the I2C1 peripheral, see i2c.h*/
static i2c_handle_t I2C1_Handle = {0};

/* temperature measurement related */
static uint8_t g_i2c_rx_buffer[APP_I2C_BUFFER_SIZE] = { 0x00 };
static uint16_t g_raw_temperature = 0x00;
static float g_celsius_temperature_1 = 0.0;

/* bluetooth transmission related */
static uint8_t g_usart_str_temperature[APP_TEMPERATURE_STRING_LENGTH] = { 0 };
static uint8_t g_usart_delimiter[APP_DELIMITER_LENGTH] = { '\r', '\n' };
static uint8_t g_usart_tx_buffer[APP_USART_TX_MESSAGE_LENGTH] = { 0 };

/***********************************************
 * INTERNAL FUNCTION PROTOTYPES
 */

/**
 * Note: 	The temperature calculation does not have to be done by the
 * 			microcontroller, it can be done on the PC. In that case,
 * 			no FPU is needed, only the raw temperature data is sent
 * 			to the PC.
 * @brief enables the FPU for calculation of temperature
 * @param void
 * @return void
 */
static void app_enable_FPU(void);

/**
 * @brief Initializes the USART for transmission to the bluetooth module (8N1)
 * @param void
 * @return void
 */
static void app_USART6_init(void);

/**
 * @brief Initializes the GPIO pins for USART function
 * @param void
 * @return void
 */
static void app_USART6_gpio_init(void);

/**
 * @brief Initializes the I2C bus for sensor communication, including GPIOs
 * @param void
 * @return void
 */
static void app_i2c_init(void);

/***********************************************
 * INTERNAL FUNCTIONS
 */

static void app_enable_FPU(void) {

	volatile uint32_t *ptr = (volatile uint32_t*)(FPU_CPACR_REGISTER);
	*ptr |= (0xf << 20);	/* activate C10 and C11 coprocessors in full access mode */

}

static void app_USART6_init(void) {

	/* Note: add get_handle */
	g_usart6_handle.p_USARTx = USART6;
	g_usart6_handle.USART_config.baudrate = USART_STD_BAUD_9600;
	g_usart6_handle.USART_config.USART_hw_flowcontrol = USART_HW_FLOW_CTRL_NONE;
	g_usart6_handle.USART_config.mode = USART_MODE_ONLY_TX;
	g_usart6_handle.USART_config.USART_no_stop = USART_STOPBITS_1;
	g_usart6_handle.USART_config.USART_wordlength = USART_WORDLEN_8BITS;
	g_usart6_handle.USART_config.USART_parity = USART_PARITY_DISABLE;
	USART_init(&g_usart6_handle);

}

static void app_USART6_gpio_init(void) {

	gpio_handle_t usart_gpios = GPIO_get_handle();

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.gpio_pin_config.GPIO_pin_mode = GPIO_MODE_ALT;
	usart_gpios.gpio_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_PP;
	usart_gpios.gpio_pin_config.GPIO_pin_pupd_control = GPIO_PIN_PU;
	usart_gpios.gpio_pin_config.GPIO_pin_speed = GPIO_SPEED_FAST;
	usart_gpios.gpio_pin_config.GPIO_alt_fun_mode = USART6_ALT_FUNCTION_MODE;

	/* TX */
	usart_gpios.gpio_pin_config.GPIO_pin_number  = GPIO_PIN_NUMBER_11;
	GPIO_init(&usart_gpios);

	/* RX */
	/*
	 * Note: we do not receive any data, only send it, in case you want
	 * to receive from bluetooth you can use this.
	usart_gpios.gpio_pin_config.GPIO_pin_number = GPIO_PIN_NUMBER_12;
	GPIO_init(&usart_gpios);
	*/
}

static void app_i2c_init(void) {

	gpio_handle_t gpiob;
	gpio_handle_t *i2c_SCL;
	gpio_handle_t *i2c_SDA;

	memset(&gpiob, 0, sizeof(gpiob));

	gpiob.pGPIOx = GPIOB;
	gpiob.gpio_pin_config.GPIO_pin_mode = GPIO_MODE_ALT;
	gpiob.gpio_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_OD;
	gpiob.gpio_pin_config.GPIO_pin_pupd_control = GPIO_NO_PULL;
	gpiob.gpio_pin_config.GPIO_alt_fun_mode = 4;
	gpiob.gpio_pin_config.GPIO_pin_speed = GPIO_SPEED_FAST;
	gpiob.gpio_pin_config.GPIO_pin_number = GPIO_PIN_NUMBER_6;

	i2c_SCL = &gpiob;
	GPIO_init(i2c_SCL);


	memset(&gpiob, 0, sizeof(gpiob));

	gpiob.pGPIOx = GPIOB;
	gpiob.gpio_pin_config.GPIO_pin_mode = GPIO_MODE_ALT;
	gpiob.gpio_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_OD;
	gpiob.gpio_pin_config.GPIO_pin_pupd_control = GPIO_NO_PULL;
	gpiob.gpio_pin_config.GPIO_alt_fun_mode = 4;
	gpiob.gpio_pin_config.GPIO_pin_speed = GPIO_SPEED_FAST;
	gpiob.gpio_pin_config.GPIO_pin_number = GPIO_PIN_NUMBER_7;

	i2c_SDA = &gpiob;

	GPIO_init(i2c_SDA);

	memset(&I2C1_Handle, 0, sizeof(I2C1_Handle));

	I2C1_Handle.pI2Cx = I2C1;
	I2C1_Handle.I2C_config.I2C_ack_control = I2C_ACK_ENABLE;
	I2C1_Handle.I2C_config.I2C_device_address = 0x66; /* not needed, but better to give value */
	I2C1_Handle.I2C_config.I2C_fm_duty_cycle = I2C_FM_DUTY_2; /* not needed */
	I2C1_Handle.I2C_config.I2C_SCL_speed = I2C_SCL_SPEED_SM;

	I2C_init(&I2C1_Handle);
}


/***********************************************
 * EXTERNAL FUNCTIONS
 */
extern void app_bluetooth_init(void) {

	RCC->APB2ENR |= (1 << 5);	/* USART6 clock enable */
	app_USART6_gpio_init();
	app_USART6_init();
	USART_peripheral_control(USART6,ENABLE);
	if(g_application_state == APP_I2C_INIT_DONE) {
		g_application_state = APP_READY;
	} else {
		g_application_state = APP_BLUETOOTH_INIT_DONE;
	}

}

extern void app_mlx_init(void) {

	app_i2c_init();
	app_enable_FPU();
	I2C_peripheral_control(I2C1, ENABLE);
	I2C_ACK(I2C1, I2C_ACK_ENABLE);
	if(g_application_state == APP_BLUETOOTH_INIT_DONE) {
		g_application_state = APP_READY;
	} else {
		g_application_state = APP_I2C_INIT_DONE;
	}

}

extern uint8_t app_get_state(void) {
	return g_application_state;
}

extern void app_mlx_measure(uint8_t slave_address) {


	uint8_t temperature_register_address = APP_TEMPERATURE_REGISTER_ADDRESS;

	g_raw_temperature = 0x00;

	i2c_master_send(&I2C1_Handle, &temperature_register_address, 1, slave_address, I2C_ENABLE_SR);
	/* get the temperature, high and low byte */
	I2C_master_recv(&I2C1_Handle, &g_i2c_rx_buffer[0], APP_I2C_BUFFER_SIZE, slave_address, I2C_DISABLE_SR);

	g_raw_temperature = ((uint16_t)g_i2c_rx_buffer[1] << 8 | (uint16_t)g_i2c_rx_buffer[0]);
	g_celsius_temperature_1 = (float)g_raw_temperature * APP_TEMPERATURE_CONVERSION_CONSTANT;
	g_celsius_temperature_1 = g_celsius_temperature_1 - APP_KELVIN_CONVERSION;

}

extern void app_mlx_send(void) {

		snprintf(g_usart_str_temperature, sizeof(g_usart_str_temperature), "%.2f", g_celsius_temperature_1);
		memcpy(g_usart_tx_buffer+APP_DELIMITER_LENGTH, g_usart_str_temperature, APP_TEMPERATURE_STRING_LENGTH);
		memcpy(g_usart_tx_buffer+0, g_usart_delimiter, APP_DELIMITER_LENGTH);
		USART_send_data(&g_usart6_handle,&g_usart_tx_buffer[0], strlen(g_usart_tx_buffer));

}

/***********************************************
 * EOF
 */

