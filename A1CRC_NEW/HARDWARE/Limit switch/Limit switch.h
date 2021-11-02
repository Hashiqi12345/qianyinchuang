#ifndef __Limit_switch_H
#define __Limit_switch_H

#define SW1 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);//牵引光栅尺限位开关
#define SW2 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10);//成角光电管限位开关
#define SW3 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11);//摆角接近开关
#define SW4 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12);//旋角接近开关

void Limit_switch(void);//初始化

#endif