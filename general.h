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
#include <stdbool.h>

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

void testButton(GPIO_TypeDef *port, uint16_t pin, uint8_t *glitch_protection,
		uint8_t *pressed, uint8_t *last_pressed, uint8_t *just_pressed);

#define isInRaisingEdge(port, pin) 			readDigitalPin(port, pin)
#define isInFallingEdge(port, pin) 			(!readDigitalPin(port, pin))
#define disablePin(port, pin)				setPinAsInput(port, pin)
#define setPinHighImpedance(port, pin)		setPinAsInput(port, pin)
#define setPinHigh(port, pin)				HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
#define setPinLow(port, pin)				HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

static inline uint8_t readDigitalPin(GPIO_TypeDef *port, uint16_t pin)
{
  if( HAL_GPIO_ReadPin(port, pin)==GPIO_PIN_SET )
	  return 1;
  return 0;
}

static inline void blinkLedInTimer(uint32_t timer_frq_hz,
		uint32_t blink_frq_hz) {
	static uint32_t frq_hz = 0, cnt;
	if (frq_hz == 0){
		if(blink_frq_hz==0) return;
		cnt = (timer_frq_hz / blink_frq_hz) >> 1;
	}
	if (++frq_hz >= cnt) {
		frq_hz = 0;
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

static inline void setPinAsInput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_DeInit(GPIOx, GPIO_Pin);
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static inline void setPinAsOutput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
		GPIO_PinState PinState) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static inline uint8_t isTimeElapsed(uint32_t *ms, uint32_t update_rate) {
	uint32_t now = HAL_GetTick();
	if ((now - *ms > update_rate) || (*ms > now)) {
		*ms = now;
		return 1;
	}
	return 0;
}

#ifdef __cplusplus
}
#endif
#endif /* INC_GENERAL_H_ */
