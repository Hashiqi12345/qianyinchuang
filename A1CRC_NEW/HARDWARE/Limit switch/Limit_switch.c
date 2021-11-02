	#include "Limit_switch.h" 
	#include "sys.h"
	
	void Limit_switch(void)
	{
		 GPIO_InitTypeDef  GPIO_InitStructure;
		
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); 
		
	   GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;	
     GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;//推挽输出
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	   GPIO_Init(GPIOG, &GPIO_InitStructure);  //初始化PC端口
	}
	