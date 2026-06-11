#include "stm32f10x.h"                  // Device header
#include "oled.h"
void shoye(){
	OLED_ShowChinese16x16(1,2,0);
	OLED_ShowChinese16x16(1,3,1);
	OLED_ShowChinese16x16(1,4,2);
	OLED_ShowChinese16x16(1,5,3);
	OLED_ShowChinese16x16(1,6,4);
	OLED_ShowChinese16x16(1,7,5);
	
	OLED_ShowChinese16x16(3,3,12);
	OLED_ShowChinese16x16(3,4,13);
	OLED_ShowChinese16x16(3,5,14);
	OLED_ShowChinese16x16(3,6,15);
}
void yushui(){
	OLED_ShowChinese16x16(1,2,16);
	OLED_ShowChinese16x16(1,3,17);
	OLED_ShowChinese16x16(1,4,23);
	OLED_ShowChinese16x16(1,5,24);
	OLED_ShowChinese16x16(1,6,25);
	OLED_ShowChinese16x16(1,7,36);	
	OLED_ShowChinese16x16(1,8,37);	
	
	OLED_ShowString(3,1,"Rain:");
}

void gunagming(){
	OLED_ShowChinese16x16(1,2,21);
	OLED_ShowChinese16x16(1,3,22);
	OLED_ShowChinese16x16(1,4,23);
	OLED_ShowChinese16x16(1,5,24);
	OLED_ShowChinese16x16(1,6,25);
	OLED_ShowChinese16x16(1,7,36);	
	OLED_ShowChinese16x16(1,8,37);	
	
	OLED_ShowString(3,1,"light:");
}

void wenshidu(){
	OLED_ShowChinese16x16(1,2,18);
	OLED_ShowChinese16x16(1,3,19);
	OLED_ShowChinese16x16(1,4,20);
	OLED_ShowChinese16x16(1,5,23);
	OLED_ShowChinese16x16(1,6,24);
	OLED_ShowChinese16x16(1,7,25);	
	
	OLED_ShowString(3,1,"temp:");
	OLED_ShowString(4,1,"humi:");
}


void chuanglikai(){
	OLED_ShowChinese16x16(2,3,26);
	OLED_ShowChinese16x16(2,4,27);
	OLED_ShowChinese16x16(2,5,28);
	OLED_ShowChinese16x16(2,6,29);
	OLED_ShowChinese16x16(2,7,30);	

}

void chuangliguan(){
	OLED_ShowChinese16x16(2,3,26);
	OLED_ShowChinese16x16(2,4,27);
	OLED_ShowChinese16x16(2,5,33);
	OLED_ShowChinese16x16(2,6,34);
	OLED_ShowChinese16x16(2,7,35);

}