#include "photosensitive.h"
#include "stm32f10x.h"

#define   RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOA
#define   GPIOx  GPIOA
#define   GPIO_PIN_x  GPIO_Pin_7    // 雨量
#define   GPIO_PIN_x1 GPIO_Pin_0    // 光敏

u16 ADC_DMA_BUF[40];

void ADC1_DMA_Init(void){
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	// --------------------- GPIO ---------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_AIN;
	gpio_init.GPIO_Pin = GPIO_PIN_x | GPIO_PIN_x1;
	GPIO_Init(GPIOx, &gpio_init);
	
	// --------------------- 先开 DMA 时钟！！！ ---------------------
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// --------------------- ADC 配置 ---------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_InitTypeDef adc_init;
	adc_init.ADC_ContinuousConvMode = ENABLE;
	adc_init.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adc_init.ADC_Mode = ADC_Mode_Independent;
	adc_init.ADC_NbrOfChannel = 2;
	adc_init.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1, &adc_init);
	
	// 通道顺序：1.光敏(CH0)  2.雨量(CH7)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 2, ADC_SampleTime_239Cycles5);
	
	// --------------------- DMA 配置（必须在 ADC 使能前！） ---------------------
	DMA_DeInit(DMA1_Channel1);
	DMA_InitTypeDef dma_init;
	dma_init.DMA_BufferSize = 40;
	dma_init.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma_init.DMA_M2M = DMA_M2M_Disable;
	dma_init.DMA_MemoryBaseAddr = (u32)ADC_DMA_BUF;
	dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_init.DMA_Mode = DMA_Mode_Circular;
	dma_init.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_init.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &dma_init);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// --------------------- 最后！使能 ADC + ADC DMA ---------------------
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
	// 校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	// 启动
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// 光敏：0,2,4  隔一个读一个
u16 Get_Light_Value(void)
{
    u32 sum = 0;
    for(u8 i=0; i<40;){

			sum += ADC_DMA_BUF[i];
			i+=2;
		}
    return sum/20;
}

// 雨量：1，3，5  隔一个读一个
u16 Get_Rain_Value(void)
{
    u32 sum = 0;
    for(u8 i=1; i<40;){
		sum += ADC_DMA_BUF[i];
			i+=2;
		}			
    return sum/20;
}