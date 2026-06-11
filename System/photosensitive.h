#ifndef __PHOTOSENSITIVE_H
#define __PHOTOSENSITIVE_H

#include "stm32f10x.h"

extern u16 ADC_DMA_BUF[40];

void ADC1_DMA_Init(void);
u16 Get_Rain_Value(void);
u16 Get_Light_Value(void);
#endif