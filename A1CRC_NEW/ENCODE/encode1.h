#ifndef __ENCODE1_H__
#define __ENCODE1_H__
#include "sys.h" //#include "stm32f10x.h"

extern uint8_t Cencode[3];

void TIM3_Mode_Config(void);
void TIM3_Init(void);
void encode1(void);
void encode1_RESET(void);
void encode1_Send_Data(void);
void encode1_limit(void);

#endif
