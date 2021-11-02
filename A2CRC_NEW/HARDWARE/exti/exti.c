#include "exti.h"
#include "delay.h"
#include "sys.h"
#include "ON_OFF.h"
#include "TIM.h"

void EXTIX_Init(void)
{
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

ON_OFF_Init(); //按键端口初始化
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //开启 AFIO 时钟
	
//GPIOB14 中断线以及中断初始化配置,上升沿触发	
GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
EXTI_InitStructure.EXTI_Line=EXTI_Line14;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure); //初始化中断线参数
	
//GPIOB15	中断线以及中断初始化配置,上升沿触发
GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);	
EXTI_InitStructure.EXTI_Line=EXTI_Line15;
EXTI_Init(&EXTI_InitStructure);

NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //使能按键外部中断通道
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级 2
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //子优先级 2
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
NVIC_Init(&NVIC_InitStructure);//初始化 NVIC
	
}


