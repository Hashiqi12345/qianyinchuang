/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "usart.h"
#include "Execute_function.h"
#include "Parse_frame_data.h" 
#include "RS485.h"
#include "function.h"
#include "delay.h"
#include "PWM.h"

void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
//
/*****************************************
�����жϽ��պ���
******************************************/
//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;  
uint16_t clr;
u8 len; 

void USART2_IRQHandler(void)                	//����1�жϷ������
	{
	u8 res;	    

 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
		{
			res =USART_ReceiveData(USART2); 	//��ȡ���յ�������
			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
			RS485_RX_CNT++;						//������������1
      if(RS485_RX_CNT > 64)RS485_RX_CNT = 0;		
		}
		
	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) //�յ�һ֡������	
		{						
			clr = USART2->SR;				
			clr = USART2->DR;
			len = RS485_RX_CNT;
			
			if(RS485_RX_BUF[1]!=0XA2)RS485_RX_CNT=0;
			else
				{
					//RS485_Send_Data(RS485_RX_BUF,len);
					FEEDBACK_PROTOCOL_CHECK();
					flag_function_configure();
				  RS485_RX_CNT=0;
				}
		}
	}	


////���߽ӽ����ز�Ƶ��
//u16 Count=0;	u16 Count1=0;

//void EXTI15_10_IRQHandler(void)
//{
//	u8 i;
//switch(i)
//{  
//case 0:	
//if(EXTI_GetITStatus(EXTI_Line14)!=RESET)//�ж�ĳ�����ϵ��ж��Ƿ���
// { 
//	 if(change_flagL==0){
//		 TIM_Cmd(TIM3,DISABLE);
//	   Count=TIM3->CNT;
//	   TIM3->CNT = 0;//��������
//	   TIM_Cmd(TIM3,ENABLE);//ʹ��TIM3,��ʼ����	
//		 }
//	 
//		 else if(change_flagL==1){
//			 GPIO_ResetBits(GPIOA,GPIO_Pin_7);

//       pack_protocol(4);
//			 RS485_Send_Data(RS485_TX_BUF,8);
//			 
//			 change_flagL=0;
//			 TIM_Cmd(TIM3,DISABLE);
//			 TIM3->CNT = 0;
//		 }
//		 EXTI_ClearITPendingBit(EXTI_Line14); //��� LINE1 �ϵ��жϱ�־λ	
// }
////break;

//case 1:	
//if(EXTI_GetITStatus(EXTI_Line15)!=RESET)//�ж�ĳ�����ϵ��ж��Ƿ���
// { 
//	 if(change_flagR==0){
//		 TIM_Cmd(TIM4,DISABLE);
//		 Count1=TIM4->CNT;
//		 //pack_protocol(1);
//		 //RS485_Send_Data(RS485_TX_BUF,8);
//	  TIM4->CNT = 0;//��������
//	  TIM_Cmd(TIM4,ENABLE);//ʹ��TIM3,��ʼ����	
//		 }
//	 
//	 else if(change_flagR==1){
//			 GPIO_ResetBits(GPIOC,GPIO_Pin_5);
//		 
//		   pack_protocol(5);
//			 RS485_Send_Data(RS485_TX_BUF,8);
//			 //delay_us(5);
//		 
//			 TIM_Cmd(TIM4,DISABLE);
//		   change_flagR=0;
//			 TIM4->CNT = 0;
//		 }
//	 EXTI_ClearITPendingBit(EXTI_Line15); //��� LINE1 �ϵ��жϱ�־
//	 //delay_ms(50);
// }
////break;
//}
//}
