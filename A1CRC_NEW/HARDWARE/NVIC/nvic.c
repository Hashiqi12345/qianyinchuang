#include "stm32f10x.h"
#include "misc.h"
#include "nvic.h"
#include "sys.h" 
#include "delay.h"
/*
中断分配说明，优先级由高向低排
顺序       类型              作用                   抢占优先级      响应优先级
 1         USART3          串口通信中断                  0                 0        
 2  EXTI0、1、4、5          Z相归0                        1                 1  
 //编码器一直处于工作状态无需匹配优先级(3 TIM4、TIM3、TIM2、TIM1   定时器的编码器模式            2                 0)      
 3         USART1          串口通信中断                  2                 0
*/

//1.-----外部限位开关信号归零  优先级 1  1            
void PG9_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)//光栅尺encode机械行程开关
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}

void PG10_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)//成角编码器encode1光电对管限位
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}

void PG11_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)//摆角编码器encode2接近开关限位
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}


void PG12_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)//旋角编码器encode3接近开关限位
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}


//3----串口1，串口3 RS485
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
	/* 配置中断使用组合  抢占式3位(0-7)，响应式1位(0-1) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
		/*外部中断Z相归零----优先级最高 1 1*/
	if(Interrupt_flag == 1)
	{

		PG9_NVIC_Config(NVIC_InitStructure);
		PG10_NVIC_Config(NVIC_InitStructure);
		PG11_NVIC_Config(NVIC_InitStructure);
		PG12_NVIC_Config(NVIC_InitStructure);
	}
	
  /* USART3中断，数据接收----优先级 0  0*/	
	if(Interrupt_flag == 2)
	{
		
		USART1_NVIC_Config(NVIC_InitStructure);
	}	
	
	/* USART1中断，数据接收----优先级 2  0*/
	if(Interrupt_flag == 3)
	{

		USART3_NVIC_Config(NVIC_InitStructure);
	}
}


