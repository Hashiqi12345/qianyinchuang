#include "exti.h"
#include "sys.h" 
#include "delay.h"

/***************************
��դ�߻�е�г̿��� PG9
�ɽǹ�����λ     PG10
�ڽǽӽ�����       PG11
���ǽӽ�����       PG12
*****************************/


void EXTI_Z_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//��դ�߻�е�г̿��� PG9
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // ��������
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* EXTI line(PG9) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource9); //�ж���1��GPIOAӳ������
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	/*----------------------------------------------------End line-------------------------------------------------------------*/
	//�ɽǹ�����λ     PG10
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // ��������
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* EXTI line(PG10) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource10); //�ж���1��GPIOAӳ������
	EXTI_InitStructure.EXTI_Line = EXTI_Line10; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

/*----------------------------------------------------End line-------------------------------------------------------------*/
	//�ڽǽӽ�����       PG11
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // ��������
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* EXTI line(PG11) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource11); //�ж���1��GPIOAӳ������
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
/*----------------------------------------------------End line-------------------------------------------------------------*/	
	//���ǽӽ�����       PG12
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // ��������
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* EXTI line(PG12) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource12); //�ж���1��GPIOAӳ������
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	
}
