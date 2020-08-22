#include "general.h"

#ifdef DEBUG_MODE
uint8_t tX_buff[TX_BUF_COL][TX_BUF_ROW], tx_buff_counter = 0;
#endif

#ifdef DEBUG_MODE
void debug_msg(uint8_t debug, uint8_t time, uint32_t counter,
		char const *format, ...) {
	if (debug == 0)
		return;
	static uint32_t debug_msg_counter = 0;
	if (counter > 0) {
		debug_msg_counter++;
		if (debug_msg_counter > counter)
			return;
	}
	va_list args;
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
#else
#define debug_msg(...)
#endif

void testButton(GPIO_TypeDef *port, uint16_t pin, uint8_t *glitch_protection,
		uint8_t *pressed, uint8_t *last_pressed, uint8_t *just_pressed) {
	if (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET) {
		*just_pressed = 1;
		if (*glitch_protection == 0) {
			*glitch_protection = 1;
			*pressed = 0;
			return;
		}
		if (*last_pressed == 1) {
			*pressed = 0;
		} else {
			*last_pressed = *pressed = 1;
		}
	} else {
		*glitch_protection = *just_pressed = *pressed = *last_pressed = 0;
	}
}
