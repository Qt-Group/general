#include "general.h"

#define TIMER_CLOCK_FREQUENCY_HZ	170000000

#ifdef DEBUG_MODE
uint8_t tX_buff[TX_BUF_COL][TX_BUF_ROW], tx_buff_counter = 0;
#endif

#ifdef DEBUG_MODE
void debug_msg(uint8_t debug, uint8_t time, uint32_t counter,
		char const *format, ...) {
	static uint32_t debug_msg_counter = 0;
	if (counter > 0) {
		debug_msg_counter++;
		if (debug_msg_counter > counter)
			return;
	}
	va_list args;
	if (debug) {
		int space = 0;
		if (time) {
			space = 5;
			sprintf(((char*) tX_buff[tx_buff_counter]), "%lu)   ",
					(uint32_t) (HAL_GetTick() / 1000));
		}
		va_start(args, format);
		if (strlen(format) < TX_BUF_ROW)
			vsprintf(((char*) tX_buff[tx_buff_counter]) + space, format, args);
		else
			vsprintf(((char*) tX_buff[tx_buff_counter]) + space,
					">=TX_BUF_ROW strlen error", args);
		rotateVal(&tx_buff_counter, 0, TX_BUF_COL - 1);
		va_end(args);
	}
}
#else
#define debug_msg(...)
#endif

void timerSetFreq(TIM_HandleTypeDef *htim, uint32_t frq_Hz) {
	const uint32_t clock_frq_hz = TIMER_CLOCK_FREQUENCY_HZ;
	debug_msg(1, 1, "timer_clock_frq=%dMHz\r\n", clock_frq_hz / 1000000U);
	uint32_t prs = 0;
	uint32_t atr;
	while (1) {
		atr = clock_frq_hz / frq_Hz / (prs + 1);
		if (((clock_frq_hz * ((uint32_t) 10)) % (frq_Hz * (prs + 1))) >= 5)
			atr++;
		if (atr < 65536) {
			atr--;
			__HAL_TIM_SET_PRESCALER(htim, prs);
			__HAL_TIM_SET_AUTORELOAD(htim, atr);
			//debug_msg(1,3,"timerSetFreq=%dHz, prs=%d, atr=%d\r\n",frq_Hz,prs,atr);
			return;
		} else {
			prs++;
		}
	}
}
