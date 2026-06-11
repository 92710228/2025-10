#ifndef __MOTOR_BUJIN_H
#define __MOTOR_BUJIN_H


/**********************************
包含头文件
**********************************/
#include "sys.h" 


/**********************************
宏定义
**********************************/
#define           MOTOR_SPEED			            1					//电机转速0-10
#define           MOTOR_ANGLE			            600				//电机旋转角度

#define MOTOR_TARGET_STEP  ((uint32_t)MOTOR_ANGLE * 4096 / 360)   // 自动计算目标步数


#define           STEP_TIM                    TIM1
#define           STEP_TIM_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define           STEP_TIM_CLK                RCC_APB2Periph_TIM1
#define           STEP_TIM_IRQ                TIM1_UP_IRQn
#define           STEP_TIM_IRQHandler         TIM1_UP_IRQHandler

#define           STEP2_GPIO_CLK              RCC_APB2Periph_GPIOB
#define           STEP2_GPIO_PORT             GPIOB
#define           STEP2_GPIO_PIN_A            GPIO_Pin_3
#define           STEP2_GPIO_PIN_B            GPIO_Pin_4
#define           STEP2_GPIO_PIN_C            GPIO_Pin_5
#define           STEP2_GPIO_PIN_D            GPIO_Pin_6
#define           STEP2_A                     PBout(3)
#define           STEP2_B                     PBout(4)
#define           STEP2_C                     PBout(5)
#define           STEP2_D                     PBout(6)


/**********************************
函数声明
**********************************/
void Step_Motor_Init(void);			//步进电机初始化
void Step_Motor_Stop(void);
void Step_Motor_Reverse(void);
void Step_Motor_Forward(void);

#endif

