#ifndef __ultrasonic_H
#define __ultrasonic_H
#include "stm32f1xx_hal.h"

#define HCSR04_Changemm 0.85

#define HCS_L hc_sr04[2].distance
#define HCS_F hc_sr04[0].distance
#define HCS_B hc_sr04[1].distance




typedef volatile struct __Hc_Sr04{

	uint8_t Hcsr04_ID;
	
	uint16_t Capture_State;//第七位为捕获完成标志，第六位为高电平捕获完成标志，低五位为溢出次数
	
	uint16_t cntStart;
	uint16_t cntContin;
	uint16_t distance;
	
	uint16_t cntArray[10];//滤波所需的数组
	uint8_t  pointArray;//滤波指针
}Hc_Sr04;

extern Hc_Sr04 hc_sr04[7];

void HcSr04_Init(Hc_Sr04 * hc_sr);

void HcSr04_CaptureCallBack(Hc_Sr04 * hcsr);

void HcSr04_Trig(void);

void HcSr04_Delay(__IO uint32_t i);

void HcSr04_NearWall(Hc_Sr04 * hcsr);

void HcSr04_USARTPrint(Hc_Sr04 * hcsr);

void Ultrasonic_Init(void);

uint16_t U1_(void);
	
uint16_t U2_(void);

uint16_t Hcsr04_Fliter(Hc_Sr04 * hcsr);


#endif
