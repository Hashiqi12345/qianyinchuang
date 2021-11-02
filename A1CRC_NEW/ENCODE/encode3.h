#ifndef __ENCODE3_H__
#define __ENCODE3_H__
#include "sys.h" //#include "stm32f10x.h"

extern uint8_t Xencode[3];

void TIM1_Mode_Config(void);
void TIM1_Init(void);
void encode3(void);
void encode3_RESET(void);
void encode3_Send_Data(void);
void encode3_limit(void);

#endif
