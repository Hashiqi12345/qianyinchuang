#include "exti.h"
#include "sys.h" 
#include "delay.h"

/***************************
光栅尺机械行程开关 PG9
成角光电管限位     PG10
摆角接近开关       PG11
旋角接近开关       PG12
*****************************/


void EXTI_Z_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//光栅尺机械行程开关 PG9
	/* config the extiline(PG9) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG| RCC_APB2Periph_AFIO,ENABLE);
	/* config the NVIC(PG9) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* EXTI line gpio config(PB1) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* EXTI line(PG9) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource9); //中断线1与GPIOA映射起来
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	/*----------------------------------------------------End line-------------------------------------------------------------*/
	//成角光电管限位     PG10
	/* config the extiline(PG10) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG| RCC_APB2Periph_AFIO,ENABLE);
	/* config the NVIC(PG10) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* EXTI line gpio config(PG10) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* EXTI line(PG10) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource10); //中断线1与GPIOA映射起来
	EXTI_InitStructure.EXTI_Line = EXTI_Line10; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

/*----------------------------------------------------End line-------------------------------------------------------------*/
	//摆角接近开关       PG11
	/* config the extiline(PG11) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG| RCC_APB2Periph_AFIO,ENABLE);
	/* config the NVIC(PA1) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* EXTI line gpio config(PG11) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* EXTI line(PG11) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource11); //中断线1与GPIOA映射起来
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
/*----------------------------------------------------End line-------------------------------------------------------------*/	
	//旋角接近开关       PG12
	/* config the extiline(PG12) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG| RCC_APB2Periph_AFIO,ENABLE);
	/* config the NVIC(PG12) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* EXTI line gpio config(PG12) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* EXTI line(PG12) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource12); //中断线1与GPIOA映射起来
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	
}
