#include "stm32f10x.h"
#include "misc.h"
#include "nvic.h"
#include "sys.h" 
#include "delay.h"
/*
�жϷ���˵�������ȼ��ɸ������
˳��       ����              ����                   ��ռ���ȼ�      ��Ӧ���ȼ�
 1         USART3          ����ͨ���ж�                  0                 0        
 2  EXTI0��1��4��5          Z���0                        1                 1  
 //������һֱ���ڹ���״̬����ƥ�����ȼ�(3 TIM4��TIM3��TIM2��TIM1   ��ʱ���ı�����ģʽ            2                 0)      
 3         USART1          ����ͨ���ж�                  2                 0
*/

//1.-----�ⲿ��λ�����źŹ���  ���ȼ� 1  1            
void PG9_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)//��դ��encode��е�г̿���
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}

void PG10_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)//�ɽǱ�����encode1���Թ���λ
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}

void PG11_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)//�ڽǱ�����encode2�ӽ�������λ
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}


void PG12_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)//���Ǳ�����encode3�ӽ�������λ
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}


//3----����1������3 RS485
void USART3_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure2)
{
	NVIC_InitStructure2.NVIC_IRQChannel = USART3_IRQn; 
	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure2);
}

void USART1_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure3)
{
	NVIC_InitStructure3.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure3.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure3.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure3.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure3);
}


void NVIC_Config(u8 Interrupt_flag)
{

  	NVIC_InitTypeDef   NVIC_InitStructure;
	/* �����ж�ʹ�����  ��ռʽ3λ(0-7)����Ӧʽ1λ(0-1) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
		/*�ⲿ�ж�Z�����----���ȼ���� 1 1*/
	if(Interrupt_flag == 1)
	{

		PG9_NVIC_Config(NVIC_InitStructure);
		PG10_NVIC_Config(NVIC_InitStructure);
		PG11_NVIC_Config(NVIC_InitStructure);
		PG12_NVIC_Config(NVIC_InitStructure);
	}
	
  /* USART3�жϣ����ݽ���----���ȼ� 0  0*/	
	if(Interrupt_flag == 2)
	{
		
		USART1_NVIC_Config(NVIC_InitStructure);
	}	
	
	/* USART1�жϣ����ݽ���----���ȼ� 2  0*/
	if(Interrupt_flag == 3)
	{

		USART3_NVIC_Config(NVIC_InitStructure);
	}
}


