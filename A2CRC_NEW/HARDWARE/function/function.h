#ifndef __function_H
#define __function_H

#include "sys.h"

extern u8 change_flagL;
extern u8 change_flagR;
extern u16 pwmL_ch1;
extern u16 pwmR_ch4;

/***********************
Push_L 0:���� 1:����	2:ֹͣ
Push_R 0:���� 1:����	2:ֹͣ
************************/
void Push_L(u8 numL); 
void Push_R(u8 numR);

/***********************
Amplitude_L 0:���� 1:���� 2:ֹͣ
Amplitude_R 0:���� 1:���� 2:ֹͣ
************************/
void Amplitude_L(u8 numL);
void Amplitude_R(u8 numR);

/***********************
MOTOR_POWER 0:������Դ 1:�رյ�Դ
Amplitude_L 0:���� 1:ֹͣ 2:��λ
Amplitude_R 0:���� 1:ֹͣ 2:��λ
************************/
void MOTOR_POWER(u8 num);
void MOTOR_L(u8 numL);
void MOTOR_R(u8 numR);

/***********************
PWM_L 0:���PWM 1:���� 2:���� 3:ָ���ٶ�
PWM_R 0:���PWM 1:���� 2:���� 3:ָ���ٶ�
************************/
void PWM_L(u8 numL);
void PWM_R(u8 numR);

void STOP(void);
void RESET_function(void);

#endif
