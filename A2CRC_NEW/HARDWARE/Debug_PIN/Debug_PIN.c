#include "stm32f10x.h"
#include "Debug_PIN.h"
#include "sys.h" 

void Debug_PIN_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PD,PG端口时钟
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;				 //IO1-->IO6端口配置
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOG,&GPIO_InitStructure);					 //根据设定参数初始化GPIOG7
 GPIO_SetBits(GPIOG,GPIO_Pin_9);						 
 GPIO_SetBits(GPIOG,GPIO_Pin_10);						
 GPIO_SetBits(GPIOG,GPIO_Pin_11);						
 GPIO_SetBits(GPIOG,GPIO_Pin_12);						 
 GPIO_SetBits(GPIOG,GPIO_Pin_13);						 
 GPIO_SetBits(GPIOG,GPIO_Pin_14);						 
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_4;				 //IO7-->IO8端口配置
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD15
 GPIO_SetBits(GPIOD,GPIO_Pin_5);						 
 GPIO_SetBits(GPIOD,GPIO_Pin_4);						 
}

