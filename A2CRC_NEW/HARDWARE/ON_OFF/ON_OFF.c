#include "sys.h" 
#include "ON_OFF.h" 

void ON_OFF_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//使能PORTE时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8;//GPIOD11,10,9,8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
  GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化 GPIOD11,10,9,8
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14|GPIO_Pin_15;//GPIOB14,15
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIOB14,15
}

