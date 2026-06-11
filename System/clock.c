#include "stm32f10x.h"
#include "delay.h"

// ========== 全局时间：真正北京时间 ==========
uint8_t hour = 12, min = 30, sec = 0; 
uint8_t tube_buf[4] = {0,0,0,0};  // 显示缓存
uint8_t tube_index = 0;           // 当前刷第几位
// ============ 引脚定义 ============
#define SDI_PIN   GPIO_Pin_5
#define CLK_PIN   GPIO_Pin_4
#define LE_PIN    GPIO_Pin_3
#define D1_PIN    GPIO_Pin_10             
#define D0_PIN    GPIO_Pin_11
#define EN_PIN    Bit_SET   
#define GPIO_PORT GPIOA
#define RCC_GPIO  RCC_APB2Periph_GPIOA       

// ============ 共阳段码表 ============
uint8_t seg_table[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// 带冒号的段码
uint8_t seg_table_colon[] = {
    0xBF, 0x86, 0xDB, 0xCF, 0xE6,
    0xED, 0xFD, 0x87, 0xFF, 0xEF
};

// ============ 位选 ============
static void Set_Channel(uint8_t idx)
{
    if (idx & 0x01) GPIO_SetBits(GPIO_PORT, D0_PIN); else GPIO_ResetBits(GPIO_PORT, D0_PIN);
    if (idx & 0x02) GPIO_SetBits(GPIO_PORT, D1_PIN); else GPIO_ResetBits(GPIO_PORT, D1_PIN);
}

// ============ GPIO 初始化 ============
void CLOCK_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_GPIO, ENABLE);
    gpio.GPIO_Pin = SDI_PIN | CLK_PIN | LE_PIN ;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_PORT, &gpio);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		GPIO_InitTypeDef gpio1;
			gpio1.GPIO_Pin =	D1_PIN | D0_PIN ;
			gpio1.GPIO_Mode = GPIO_Mode_Out_PP;
			gpio1.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &gpio1);
	
    GPIO_ResetBits(GPIO_PORT, CLK_PIN);
  //  GPIO_SetBits(GPIO_PORT, EN_PIN);
    GPIO_ResetBits(GPIO_PORT, LE_PIN);
}

// ============ 发送数据 ============
static void CLOCK_Send_Seg(uint8_t dataH, uint8_t dataL)
{
    uint16_t val = ((uint16_t)dataH << 8) | dataL;
    for (uint8_t i = 0; i < 16; i++) {
        GPIO_ResetBits(GPIO_PORT, CLK_PIN);
        if (val & 0x8000) GPIO_SetBits(GPIO_PORT, SDI_PIN);
        else GPIO_ResetBits(GPIO_PORT, SDI_PIN);
        val <<= 1;
        GPIO_SetBits(GPIO_PORT, CLK_PIN);
    }
}

// ============================
// RTC 初始化（真正北京时间）
// ============================
void RTC_INIT(uint32_t total_sec)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    // 只在第一次设置时间时复位，平时不要复位！
    if( BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5 )
    {
        BKP_DeInit();
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5); // 标记已设置过时间

        RCC_LSICmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
        RCC_RTCCLKCmd(ENABLE);
        RTC_WaitForSynchro();
        RTC_SetPrescaler(32767);
        RTC_SetCounter(total_sec);
    }
}

// ============================
// 读取当前北京时间
// ============================
void RTC_GetTime_Run(void)
{
    uint32_t t = RTC_GetCounter();
    sec = t % 60;
    min = (t / 60) % 60;
    hour = (t / 3600) % 24;
}

// ============================
// TIM2 中断初始化（10ms刷新一次数码管）
// ============================
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef tim;
    NVIC_InitTypeDef nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 1ms 中断一次
    tim.TIM_Period = 9;
    tim.TIM_Prescaler = 7199;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority =3;  // 高优先级
    nvic.NVIC_IRQChannelSubPriority = 3;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    TIM_Cmd(TIM2, ENABLE);
}


// ============================
// 定时器中断服务函数
// 功能：自动刷新时间 + 自动刷新数码管
// ============================
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        // 1. 每 1ms 刷新一位数码管
        Set_Channel(tube_index);
        CLOCK_Send_Seg(tube_buf[tube_index], 0x00);

        GPIO_SetBits(GPIO_PORT, LE_PIN);
        Delay_us(1);
        GPIO_ResetBits(GPIO_PORT, LE_PIN);

        tube_index++;
        if(tube_index >= 4) tube_index = 0;

        // 2. 每 500ms 更新一次时间（不影响显示）
        static uint16_t rtc_cnt = 0;
        rtc_cnt++;
        if(rtc_cnt >= 500)
        {
            rtc_cnt = 0;
            RTC_GetTime_Run();

            // 更新显示缓存
            tube_buf[0] = seg_table[min / 10];
            tube_buf[1] = seg_table_colon[min % 10]; // 带冒号
            tube_buf[2] = seg_table[sec / 10];
            tube_buf[3] = seg_table[sec % 10];
        }
    }
}