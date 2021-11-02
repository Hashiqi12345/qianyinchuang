#include "2803.h"
#include "sys.h"	
#include "stm32f10x.h"

//2803_0
void OUT_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2;				 //out1-->PG7 �˿�����
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOG,&GPIO_InitStructure);					 //�����趨������ʼ��GPIOG7
 GPIO_ResetBits(GPIOG,GPIO_Pin_7);						 //PG7 �����
 GPIO_ResetBits(GPIOG,GPIO_Pin_6);						 //PG6 �����
 GPIO_ResetBits(GPIOG,GPIO_Pin_5);						 //PG5 �����
 GPIO_ResetBits(GPIOG,GPIO_Pin_4);						 //PG4 �����
 GPIO_ResetBits(GPIOG,GPIO_Pin_3);						 //PG3 �����
 GPIO_ResetBits(GPIOG,GPIO_Pin_2);						 //PG2 �����
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15|GPIO_Pin_14;				 //out7-->PD15 �˿�����
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOD15
 GPIO_ResetBits(GPIOD,GPIO_Pin_15);						 //PD15 �����
 GPIO_ResetBits(GPIOD,GPIO_Pin_14);						 //PD16 �����	
}

//2803_1
void out_Init(void)
{GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;				 //out1_1-->PC9 �˿�����
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC,&GPIO_InitStructure);					 //�����趨������ʼ��GPIOC9
 GPIO_ResetBits(GPIOC,GPIO_Pin_9);						 //PC9 �����
 GPIO_ResetBits(GPIOC,GPIO_Pin_8);						 //PC8 �����
 GPIO_ResetBits(GPIOC,GPIO_Pin_7);						 //PC7 �����
 GPIO_ResetBits(GPIOC,GPIO_Pin_6);						 //PC6 �����
	
}
