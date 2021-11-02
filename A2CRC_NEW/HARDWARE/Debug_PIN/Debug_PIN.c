#include "stm32f10x.h"
#include "Debug_PIN.h"
#include "sys.h" 

void Debug_PIN_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PD,PG�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;				 //IO1-->IO6�˿�����
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOG,&GPIO_InitStructure);					 //�����趨������ʼ��GPIOG7
 GPIO_SetBits(GPIOG,GPIO_Pin_9);						 
 GPIO_SetBits(GPIOG,GPIO_Pin_10);						
 GPIO_SetBits(GPIOG,GPIO_Pin_11);						
 GPIO_SetBits(GPIOG,GPIO_Pin_12);						 
 GPIO_SetBits(GPIOG,GPIO_Pin_13);						 
 GPIO_SetBits(GPIOG,GPIO_Pin_14);						 
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_4;				 //IO7-->IO8�˿�����
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOD15
 GPIO_SetBits(GPIOD,GPIO_Pin_5);						 
 GPIO_SetBits(GPIOD,GPIO_Pin_4);						 
}

