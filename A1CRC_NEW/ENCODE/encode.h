#ifndef __ENCODE_H__
#define __ENCODE_H__
#include "sys.h" 

extern uint8_t Gencode[2];
extern u8 mode;
extern u8 distance;
	
void TIM4_Mode_Config(void);//ƒ£ Ω≈‰÷√
void TIM4_Init(void);//≥ı º
void encode(void);
void encode_RESET(void);
void encode_Send_Data(void);
void encode_limit(u8 mode);
	
#endif
