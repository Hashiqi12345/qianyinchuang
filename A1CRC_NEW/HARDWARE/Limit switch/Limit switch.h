#ifndef __Limit_switch_H
#define __Limit_switch_H

#define SW1 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);//ǣ����դ����λ����
#define SW2 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10);//�ɽǹ�����λ����
#define SW3 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11);//�ڽǽӽ�����
#define SW4 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12);//���ǽӽ�����

void Limit_switch(void);//��ʼ��

#endif