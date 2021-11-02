#include "PWM.h"

void PWM_PIN_INIT(void)
	{
		
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
		
		/************
		 IN1 PA7 :0 1 0 1 
		 IN2 PC4 :0 0 1 1
		 IN3 PC5 :0 1 0 1
		 IN4 PB0 :0 0 1 1
		*************/
		//IN1
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;				
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 
    GPIO_ResetBits(GPIOA,GPIO_Pin_7);						 
		//IN4
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;				
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		
    GPIO_Init(GPIOB,&GPIO_InitStructure);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);		
    //IN2 IN3
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;				 
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 		 
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOC,&GPIO_InitStructure);
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);
		GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	}
	
/************************
	定时器PWM模式设置
************************/
void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  
  
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); 
 
	TIM_Cmd(TIM1, ENABLE);  
	TIM_ARRPreloadConfig(TIM1, ENABLE); 
	TIM_CtrlPWMOutputs(TIM1,ENABLE);      
	}
