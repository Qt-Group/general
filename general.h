/*
 * general.h
 *
 *  Created on: Aug 20, 2020
 *  Author: a.rostamzadeh@gmail.com
 */

#ifndef INC_GENERAL_H_
#define INC_GENERAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "usart.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef DEBUG_MODE
#define  TX_BUF_COL     50  //max number of messages
#define  TX_BUF_ROW     100 //max length of each message
#if (TX_BUF_COL < 1 || TX_BUF_COL > 254)
#error "TX_BUF_COL is out of range!"
#endif
#if (TX_BUF_ROW < 1 || TX_BUF_ROW > 254)
#error "TX_BUF_ROW is out of range!"
#endif	
extern uint8_t tX_buff[TX_BUF_COL][TX_BUF_ROW], tx_buff_counter;
#endif

#ifdef DEBUG_MODE
void debug_msg(uint8_t debug, uint8_t time, uint32_t counter,
		char const *format, ...) ;
#endif	
	
void timerSetFreq(TIM_HandleTypeDef *htim, uint32_t frq_Hz);

static inline void blinkLED(void) {
	static uint8_t one_hz = 0;
	if (++one_hz >= 10) { // 1 Hz
		one_hz = 0;
		HAL_GPIO_TogglePin(StatusLED_GPIO_Port, StatusLED_Pin);
	}
}

#ifdef DEBUG_MODE
static inline void rotateVal(uint8_t *val, uint8_t min, uint8_t max) {
	(*val)++;
	if (*val > max)
		*val = min;
}
#endif
	
#ifdef DEBUG_MODE
static inline void sendTxBuffer(void) {
	static unsigned char last_tx_buff_counter = 0;

	while (last_tx_buff_counter != tx_buff_counter) {
		HAL_UART_Transmit(&huart1, tX_buff[last_tx_buff_counter],
				strlen((const char*) tX_buff[last_tx_buff_counter]), 1000);
		rotateVal(&last_tx_buff_counter, 0, TX_BUF_COL - 1);
	}
}
#endif

#ifdef __cplusplus
}
#endif
#endif /* INC_GENERAL_H_ */
