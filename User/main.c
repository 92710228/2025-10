#include "stm32f10x.h"
#include "clock.h"
#include "delay.h"
#include "oled.h"
#include "stepmotor.h"
#include "dht11.h"
#include "led.h"//字模
#include "photosensitive.h"
#include "usart_it.h"
extern volatile uint8_t num,qing;
extern volatile _Bool flag_bujin_foreward,flag_bujin_reversal;
void qingping();

int a=0;
int main(void)
{
	//已完成：DHT11,光敏，雨水，语言模块接单片机
//		CLOCK_GPIO_Init();
//    RTC_INIT(12*3600 + 0*60 + 0);
//		TIM2_Init();
	//时钟部分
		ADC1_DMA_Init();
		while(DHT11_Init());
		OLED_Init();
		Step_Motor_Init();
		USART_init();
uint16_t temp,humi;
	shoye();
    while(1)
    {

			switch(num){
				case 1:{	
					if(Get_Rain_Value()>1000|Get_Light_Value()>3000){num=6;}
					qingping();
					yushui();
				OLED_ShowNum(3,6,Get_Rain_Value(),4);
					break;
				}
				case 2:{	
					if(Get_Rain_Value()>1000|Get_Light_Value()>3000){num=6;}
					qingping();
					wenshidu();
				Dht11_Get_Temp_Humi_Value(&temp,&humi);
				OLED_ShowNum(4,6,(humi/10)+10,2);
				OLED_ShowNum(3,6,(temp/10)-25,2);
					break;
				}
				case 3:{	
					if(Get_Rain_Value()>1000|Get_Light_Value()>3000){num=6;}
					qingping();
					gunagming();
				OLED_ShowNum(3,7,Get_Light_Value(),4);
					break;
				}
				case 4:{	
					qingping();
					chuanglikai();
					Step_Motor_Reverse();
					while(flag_bujin_reversal);
					a=0;
					num=6;
					break;
				}
				case 5:{	
					qingping();
					chuangliguan();
					if(a==1){
							break;
					}
					else{
					Step_Motor_Forward();
					while(flag_bujin_foreward);
					}
					num=6;
					break;
				}
				case 6:{
				if(Get_Rain_Value()>1000){
											qingping();
											chuangliguan();
									if(a==0){
											Step_Motor_Forward();
											while(flag_bujin_foreward);	
										a=1;
									}										
								}
									//晚上
				if(Get_Light_Value()>3000){
											qingping();
											chuangliguan();
									if(a==0){
											Step_Motor_Forward();
											while(flag_bujin_foreward);	
										a=1;
									}	
								}
				break;}
				default:{
					qingping();
										//有雨水
								if(Get_Rain_Value()>1000){
											qingping();
											chuangliguan();
									if(a==0){
											Step_Motor_Forward();
											while(flag_bujin_foreward);	
										a=1;
									}										
								}
									//晚上
									if(Get_Light_Value()>3000){
											qingping();
											chuangliguan();
									if(a==0){
											Step_Motor_Forward();
											while(flag_bujin_foreward);	
										a=1;
									}	
								}

				}
				}
   }
}
void qingping(){
		if(qing==1){
		OLED_Clear();
		Delay_ms(100);
		qing=0;
		}	
}