#ifndef __EXTI_H__
#define __EXTI_H__
#include "sys.h" 
#include "stm32f10x.h"

#define SW GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
#define SW1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
#define SW2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
#define SW3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);

void EXTI_Z_Config(void);//±‡¬Î∆˜Zœ‡πÈ¡„

#endif
