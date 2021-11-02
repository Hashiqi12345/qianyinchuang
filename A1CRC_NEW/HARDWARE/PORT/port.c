#include "port.h"

 /***************  
 配置PORT用到的I/O口 
 *******************/
void PORT_GPIO_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // 使能PD端口时钟  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); // 使能PG端口时钟  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  //初始化PC端口
	GPIO_SetBits(GPIOD, GPIO_Pin_14);
	GPIO_SetBits(GPIOD, GPIO_Pin_15);
	GPIO_ResetBits(GPIOD, GPIO_Pin_8);	
	GPIO_ResetBits(GPIOD, GPIO_Pin_9);
	GPIO_ResetBits(GPIOD, GPIO_Pin_10);
	GPIO_ResetBits(GPIOD, GPIO_Pin_11);

	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //初始化PC端口
	GPIO_SetBits(GPIOG, GPIO_Pin_2);
	GPIO_SetBits(GPIOG, GPIO_Pin_3);
	GPIO_SetBits(GPIOG, GPIO_Pin_4);
	GPIO_SetBits(GPIOG, GPIO_Pin_5);
	GPIO_SetBits(GPIOG, GPIO_Pin_6);
	GPIO_SetBits(GPIOG, GPIO_Pin_7);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PC端口
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);

}


void ON_OFF_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//使能PORTE时钟
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9;//GPIOD11,10,9,8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
  GPIO_Init(GPIOG, &GPIO_InitStructure); //初始化 GPIOD11,10,9,8
}
