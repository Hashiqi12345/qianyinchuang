#ifndef __function_H
#define __function_H

#include "sys.h"

extern u8 change_flagL;
extern u8 change_flagR;
extern u16 pwmL_ch1;
extern u16 pwmR_ch4;

/***********************
Push_L 0:向上 1:向下	2:停止
Push_R 0:向上 1:向下	2:停止
************************/
void Push_L(u8 numL); 
void Push_R(u8 numR);

/***********************
Amplitude_L 0:向上 1:向下 2:停止
Amplitude_R 0:向上 1:向下 2:停止
************************/
void Amplitude_L(u8 numL);
void Amplitude_R(u8 numR);

/***********************
MOTOR_POWER 0:开启电源 1:关闭电源
Amplitude_L 0:启动 1:停止 2:复位
Amplitude_R 0:启动 1:停止 2:复位
************************/
void MOTOR_POWER(u8 num);
void MOTOR_L(u8 numL);
void MOTOR_R(u8 numR);

/***********************
PWM_L 0:输出PWM 1:加速 2:减速 3:指定速度
PWM_R 0:输出PWM 1:加速 2:减速 3:指定速度
************************/
void PWM_L(u8 numL);
void PWM_R(u8 numR);

void STOP(void);
void RESET_function(void);

#endif
