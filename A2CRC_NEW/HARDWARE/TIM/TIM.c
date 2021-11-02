#include "sys.h"
#include "TIM.h"

void TIM3_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	
  TIM_TimeBaseInitStructure.TIM_Period =49999 ;// �Զ���װ�ؼĴ������ڵ�ֵ(��ʱʱ��)
	TIM_TimeBaseInitStructure.TIM_Prescaler=7199;  //ʱ��Ԥ��Ƶ��   ���� ��ʱ��Ƶ��=72/(ʱ��Ԥ��Ƶ+1)
	//((1+TIM_Prescaler )/72M)*(1+TIM_Period )  
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //��ʱ��ģʽ  ���ϼ���  
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ��ָ�ֵ 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
}

void TIM4_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 
	
  TIM_TimeBaseInitStructure.TIM_Period =49999 ;// �Զ���װ�ؼĴ������ڵ�ֵ(��ʱʱ��)
	TIM_TimeBaseInitStructure.TIM_Prescaler=7199;  //ʱ��Ԥ��Ƶ��   ���� ��ʱ��Ƶ��=72/(ʱ��Ԥ��Ƶ+1)
	//((1+TIM_Prescaler )/72M)*(1+TIM_Period )  
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //��ʱ��ģʽ  ���ϼ���  
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ��ָ�ֵ 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);	
}
