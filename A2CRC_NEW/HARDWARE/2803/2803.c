#include "2803.h"
#include "sys.h"	
#include "stm32f10x.h"

//2803_0
void OUT_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2;				 //out1-->PG7 端口配置
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOG,&GPIO_InitStructure);					 //根据设定参数初始化GPIOG7
 GPIO_ResetBits(GPIOG,GPIO_Pin_7);						 //PG7 输出高
 GPIO_ResetBits(GPIOG,GPIO_Pin_6);						 //PG6 输出高
 GPIO_ResetBits(GPIOG,GPIO_Pin_5);						 //PG5 输出高
 GPIO_ResetBits(GPIOG,GPIO_Pin_4);						 //PG4 输出高
 GPIO_ResetBits(GPIOG,GPIO_Pin_3);						 //PG3 输出高
 GPIO_ResetBits(GPIOG,GPIO_Pin_2);						 //PG2 输出高
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15|GPIO_Pin_14;				 //out7-->PD15 端口配置
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD15
 GPIO_ResetBits(GPIOD,GPIO_Pin_15);						 //PD15 输出高
 GPIO_ResetBits(GPIOD,GPIO_Pin_14);						 //PD16 输出高	
}

//2803_1
void out_Init(void)
{GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;				 //out1_1-->PC9 端口配置
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC,&GPIO_InitStructure);					 //根据设定参数初始化GPIOC9
 GPIO_ResetBits(GPIOC,GPIO_Pin_9);						 //PC9 输出高
 GPIO_ResetBits(GPIOC,GPIO_Pin_8);						 //PC8 输出高
 GPIO_ResetBits(GPIOC,GPIO_Pin_7);						 //PC7 输出高
 GPIO_ResetBits(GPIOC,GPIO_Pin_6);						 //PC6 输出高
	
}
