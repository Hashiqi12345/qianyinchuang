#ifndef __ENCODE2_H__
#define __ENCODE2_H__
#include "sys.h" //#include "stm32f10x.h"

extern uint8_t Bencode[3];

void TIM2_Mode_Config(void);
void TIM2_Init(void);
void encode2(void);
void encode2_RESET(void);
void encode2_Send_Data(void);
void encode2_limit(void);
void user_Encode2_ResetL(void);
void user_Encode2_ResetR(void);

#endif
