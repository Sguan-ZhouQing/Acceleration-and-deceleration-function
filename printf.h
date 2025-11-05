#ifndef __PRINTF_H
#define __PRINTF_H

#include "stm32g4xx_hal.h"

#define G431_BUFFER_SIZE 256
extern uint8_t DataBuff[200];
extern uint16_t RxLine;
extern float Adjustable_Data;

void UART_Init(void);

#endif // PRINTF_H

