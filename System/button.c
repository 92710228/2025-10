#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"

void button_init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef gpio_init;
		gpio_init.GPIO_Mode=GPIO_Mode_Out_OD;
		gpio_init.GPIO_Pin=GPIO_Pin_0;
		gpio_init.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&gpio_init);
}
void button_down(){

	
}
	

