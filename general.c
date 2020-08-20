#include "general.h"

#define TIMER_CLOCK_FREQUENCY_HZ	170000000

#ifdef DEBUG_MODE
uint8_t tX_buff[TX_BUF_COL][TX_BUF_ROW], tx_buff_counter = 0;
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
