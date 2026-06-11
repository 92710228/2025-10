#include "stm32f10x.h"                  // Device header
#include "usart_it.h"

volatile uint8_t num=0,qing=0;
void USART_init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	GPIO_InitTypeDef gpio_init;
		gpio_init.GPIO_Mode=GPIO_Mode_AF_PP;
		gpio_init.GPIO_Pin=GPIO_Pin_9;
		gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_init);
	
		gpio_init.GPIO_Mode=GPIO_Mode_IPU;
		gpio_init.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&gpio_init);
	
	USART_InitTypeDef usart_init;
		usart_init.USART_BaudRate=9600;
		usart_init.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
		usart_init.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
		usart_init.USART_Parity=USART_Parity_No;
		usart_init.USART_StopBits=USART_StopBits_1;
		usart_init.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&usart_init);
	
	// 使能 USART1 接收中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_InitTypeDef nvic_init;
		nvic_init.NVIC_IRQChannel=USART1_IRQn;
		nvic_init.NVIC_IRQChannelCmd=ENABLE;
		nvic_init.NVIC_IRQChannelPreemptionPriority=1;
		nvic_init.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&nvic_init);
	
	USART_Cmd(USART1,ENABLE);
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
		qing=1;
		num=(uint8_t)USART_ReceiveData(USART1);
		
	}
}