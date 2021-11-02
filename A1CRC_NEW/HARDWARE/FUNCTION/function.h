#ifndef __FUNCTION_H
#define	__FUNCTION_H

#include "stm32f10x.h"

void DISTANCE_function(u8 num);
void ANGLE1_function(u8 num1);
void ANGLE2_function(u8 num2);
void ANGLE3_function(u8 num3);
void STOP(void);
void RESET_PLACE(void);
void ENCODE_function(u8 num);
void AngleAreaData_Save(void);
void Initial_Reset(void);

void Baijiao_function(void);
void Xuanjiao_function(void);
void QianYin_function(void);
#endif 
