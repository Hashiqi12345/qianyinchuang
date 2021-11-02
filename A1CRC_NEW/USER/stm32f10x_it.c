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
#include "delay.h"
#include "port.h"	
#include "Execute_function.h"
#include "Parse_frame_data.h"
#include "RS485.h"
#include "encode.h"
#include "encode1.h"
#include "encode2.h"
#include "encode3.h"

 u8 Z=0;
 u8 X=0;
 u8 C=0;
  u8 V=0;
 
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

/*****************************************
串口中断接收函数
******************************************/
//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;  
uint16_t clr;
u8 PROTOCOL[64];
u8 len; 

void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 res;	    

 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
		{
			res =USART_ReceiveData(USART2); 	//读取接收到的数据
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1				
		}
		
	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) //收到一帧的数据	
		{						
			clr = USART2->SR;				
			clr = USART2->DR;
			len = RS485_RX_CNT;
			
			if(RS485_RX_BUF[1]!=0XA1)RS485_RX_CNT=0;
			else
				{
					//RS485_Send_Data(RS485_RX_BUF,len);
					FEEDBACK_PROTOCOL_CHECK();
					flag_function_configure();
				  RS485_RX_CNT=0;
				}
		}
		delay_us(4);
	}	

/*----------------------------------------------------End line-------------------------------------------------------------*/	

////定时器4，光栅尺，Z相归零        
//void EXTI9_5_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)   //判断1是否发生中断
//	{	
//	 GPIO_SetBits(GPIOD,GPIO_Pin_14);	
//		
//	TIM_Cmd(TIM4, DISABLE );	
//	TIM4->CNT = 0;//每次遇到相对零(Z信号)就将计数归0
//		
//	Gencode[0]=0;	Gencode[1]=0;
//	pack_protocol(4);
//	RS485_Send_Data(RS485_TX_BUF,8);	
//		
//	TIM_Cmd(TIM4, ENABLE);
//	}
//	EXTI_ClearITPendingBit(EXTI_Line9);       //清除中断标志											 
//}

/*----------------------------------------------------End line-------------------------------------------------------------*/	

////定时器3  Z相归零         
//	void EXTI15_10_IRQHandler(void)
//		{
//			if(EXTI_GetITStatus(EXTI_Line10)!=RESET)   //判断1是否发生中断
//				{
//					TIM_Cmd(TIM3, DISABLE );
//					TIM3->CNT = 0;//每次遇到相对零(Z信号)就将计数归0
//					
//					Cencode[1]=1;	Cencode[2]=1;
//	        pack_protocol(1);
//	        RS485_Send_Data(RS485_TX_BUF,8);
//					
//					TIM_Cmd(TIM3, ENABLE);
//				
//				EXTI_ClearITPendingBit(EXTI_Line10);       //清除中断标志												 
//			}
///*----------------------------------------------------End line-------------------------------------------------------------*/	

////定时器2  Z相归零        
//			else if(EXTI_GetITStatus(EXTI_Line11)!=RESET)   //判断1是否发生中断
//				{
//					TIM_Cmd(TIM2, DISABLE );
//					TIM2->CNT = 0;//每次遇到相对零(Z信号)就将计数归0
//					
//					Bencode[1]=0;	Bencode[2]=0;
//	        pack_protocol(2);
//	        RS485_Send_Data(RS485_TX_BUF,8);
//					
//					TIM_Cmd(TIM2, ENABLE);
//				
//				EXTI_ClearITPendingBit(EXTI_Line11);       //清除中断标志											 
//       }
///*----------------------------------------------------End line-------------------------------------------------------------*/

////定时器1  Z相归零			
//		else if(EXTI_GetITStatus(EXTI_Line12)!=RESET)   //判断1是否发生中断
//				{
//					TIM_Cmd(TIM1, DISABLE );
//					TIM1->CNT = 0;//每次遇到相对零(Z信号)就将计数归0
//					
//					Xencode[1]=0;	Xencode[2]=0;
//	        pack_protocol(3);
//	        RS485_Send_Data(RS485_TX_BUF,8);
//					
//					TIM_Cmd(TIM1, ENABLE);
//				
//				EXTI_ClearITPendingBit(EXTI_Line12);       //清除中断标志									 
//				}
//		}
////}