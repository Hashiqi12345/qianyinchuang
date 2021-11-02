#include "sys.h"
#include "TIM.h"

void TIM3_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	
  TIM_TimeBaseInitStructure.TIM_Period =49999 ;// 自动重装载寄存器周期的值(定时时间)
	TIM_TimeBaseInitStructure.TIM_Prescaler=7199;  //时钟预分频数   例如 ：时钟频率=72/(时钟预分频+1)
	//((1+TIM_Prescaler )/72M)*(1+TIM_Period )  
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //定时器模式  向上计数  
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时间分割值 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
}

void TIM4_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 
	
  TIM_TimeBaseInitStructure.TIM_Period =49999 ;// 自动重装载寄存器周期的值(定时时间)
	TIM_TimeBaseInitStructure.TIM_Prescaler=7199;  //时钟预分频数   例如 ：时钟频率=72/(时钟预分频+1)
	//((1+TIM_Prescaler )/72M)*(1+TIM_Period )  
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //定时器模式  向上计数  
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时间分割值 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);	
}
