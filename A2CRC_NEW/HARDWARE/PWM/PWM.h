#ifndef _PWM_H
#define _PWM_H
#include "sys.h"


#define IN1 PAout(7)
#define IN2 PCout(4)
#define IN3 PCout(5)
#define IN4 PBout(0)

void PWM_PIN_INIT(void);
void TIM1_PWM_Init(u16 arr,u16 psc);

#endif

