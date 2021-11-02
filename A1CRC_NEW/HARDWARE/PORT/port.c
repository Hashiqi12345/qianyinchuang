#include "port.h"

 /***************  
 ����PORT�õ���I/O�� 
 *******************/
void PORT_GPIO_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // ʹ��PD�˿�ʱ��  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); // ʹ��PG�˿�ʱ��  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  //��ʼ��PC�˿�
	GPIO_SetBits(GPIOD, GPIO_Pin_14);
	GPIO_SetBits(GPIOD, GPIO_Pin_15);
	GPIO_ResetBits(GPIOD, GPIO_Pin_8);	
	GPIO_ResetBits(GPIOD, GPIO_Pin_9);
	GPIO_ResetBits(GPIOD, GPIO_Pin_10);
	GPIO_ResetBits(GPIOD, GPIO_Pin_11);

	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //��ʼ��PC�˿�
	GPIO_SetBits(GPIOG, GPIO_Pin_2);
	GPIO_SetBits(GPIOG, GPIO_Pin_3);
	GPIO_SetBits(GPIOG, GPIO_Pin_4);
	GPIO_SetBits(GPIOG, GPIO_Pin_5);
	GPIO_SetBits(GPIOG, GPIO_Pin_6);
	GPIO_SetBits(GPIOG, GPIO_Pin_7);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PC�˿�
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);

}


void ON_OFF_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//ʹ��PORTEʱ��
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9;//GPIOD11,10,9,8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
  GPIO_Init(GPIOG, &GPIO_InitStructure); //��ʼ�� GPIOD11,10,9,8
}
