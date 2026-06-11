#ifndef __CLOCK_H
#define __CLOCK_H

#include "stm32f10x.h"

extern uint8_t hour, min, sec;

void CLOCK_GPIO_Init(void);
void RTC_INIT(uint32_t total_sec);
void TIM2_Init(void);
#endif
