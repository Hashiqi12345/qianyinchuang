#include "exti.h"
#include "delay.h"
#include "sys.h"
#include "ON_OFF.h"
#include "TIM.h"

void EXTIX_Init(void)
{
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

ON_OFF_Init(); //�����˿ڳ�ʼ��
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //���� AFIO ʱ��
	
//GPIOB14 �ж����Լ��жϳ�ʼ������,�����ش���	
GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
EXTI_InitStructure.EXTI_Line=EXTI_Line14;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure); //��ʼ���ж��߲���
	
//GPIOB15	�ж����Լ��жϳ�ʼ������,�����ش���
GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);	
EXTI_InitStructure.EXTI_Line=EXTI_Line15;
EXTI_Init(&EXTI_InitStructure);

NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //ʹ�ܰ����ⲿ�ж�ͨ��
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ� 2
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //�����ȼ� 2
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure);//��ʼ�� NVIC
	
}


