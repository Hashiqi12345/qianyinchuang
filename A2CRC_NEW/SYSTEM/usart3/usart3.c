#include "stm32f10x.h"
#include "USART3.h"
#include "sys.h" 
#include "ON_OFF.h"
#include "2803.h"  
#include "Debug_PIN.h"
#include "adc.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 



//该串口用于与触摸屏通讯


//如果使用ucos,则包括下面的头文件即可.

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

//FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
//重定义fputc函数 
//int fputc(int ch, FILE *f)
//{      
	//while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
  // USART3->DR = (u8) ch;      
//return ch;
//}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	  
  
void uart3_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	
	//USART3_TX   GPIOB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB10
   
  //USARTB_RX	  GPIOB11初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB11 

  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口1 

}

//串口接收由触摸屏发出的指令，uart3_receive接收到的地址为触摸屏上虚拟对应地址
void USART3_IRQHandler(void)                	//串口3中断服务程序
	{
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断3
		{
			static	int uart3_receive=0;
			u8 i=0;
		  uart3_receive =USART_ReceiveData(USART3);	//读取接收到的数据
			switch(i)
		{  
    //LU 磁性L->电磁阀L1
	  case 0:
			if(ON_OFF3==0)
				{
					if(uart3_receive==0xA1)GPIO_SetBits(GPIOG, GPIO_Pin_2);
					else if(uart3_receive==0xA0)GPIO_ResetBits(GPIOG, GPIO_Pin_2);	
				}
				else GPIO_ResetBits(GPIOG, GPIO_Pin_2);	
				break;

	  //LD 磁性L->电磁阀L2				
		case 1:
			if(uart3_receive==0xA3)GPIO_SetBits(GPIOG, GPIO_Pin_3);
      else if(uart3_receive==0xA2)GPIO_ResetBits(GPIOG, GPIO_Pin_3);break;

    //RU 磁性R->电磁阀R1		
	  case 2:
			if(ON_OFF3==0)
				{
					if(uart3_receive==0xA5)GPIO_SetBits(GPIOD, GPIO_Pin_14);
					else if(uart3_receive==0xA4)GPIO_ResetBits(GPIOD, GPIO_Pin_14);	
				}
				else GPIO_ResetBits(GPIOD, GPIO_Pin_14);	break;

	  //RD 磁性R->电磁阀L2					
		case 3:
			if(uart3_receive==0xA7)GPIO_SetBits(GPIOD, GPIO_Pin_15);
      else if(uart3_receive==0xA6)GPIO_ResetBits(GPIOD, GPIO_Pin_15);			break;
			
	  //LL 二线L->小电机L 正			   	
		case 4:
			if(ON_OFF1==0)
				{
					if(uart3_receive==0xB1)GPIO_SetBits(GPIOC, GPIO_Pin_9);
					else if(uart3_receive==0xB0)GPIO_ResetBits(GPIOC, GPIO_Pin_9);
				}
				else GPIO_ResetBits(GPIOC, GPIO_Pin_9);break;
				
		//LR 二线L->小电机L 反			
	  case 5:
		  if(uart3_receive==0xB3)GPIO_SetBits(GPIOC, GPIO_Pin_8);
	    else if(uart3_receive==0xB2)GPIO_ResetBits(GPIOC, GPIO_Pin_8);break;
		
		//RL 二线R->小电机R 正			   	
		case 6:
			if(ON_OFF2==0)
				{
					if(uart3_receive==0xB5)GPIO_SetBits(GPIOC, GPIO_Pin_6);
					else if(uart3_receive==0xB4)GPIO_ResetBits(GPIOC, GPIO_Pin_6);
				}
				else GPIO_ResetBits(GPIOC, GPIO_Pin_6);break;
				
		//RR 二线R->小电机R 反			
	  case 7:
		  if(uart3_receive==0xB7)GPIO_SetBits(GPIOC, GPIO_Pin_7);
	    else if(uart3_receive==0xB6)GPIO_ResetBits(GPIOC, GPIO_Pin_7);break;
		
		//L 大电机L继电器	
		case 8:
			if(uart3_receive==0xC1)GPIO_SetBits(GPIOG, GPIO_Pin_5);
		  else if(uart3_receive==0xC0)GPIO_ResetBits(GPIOG, GPIO_Pin_5);break;
		
		//L DC+
	  case 9:
	    if(uart3_receive==0xE0)GPIO_ResetBits(GPIOD, GPIO_Pin_4);
      else if(uart3_receive==0xE1)
				{
				 GPIO_SetBits(GPIOD, GPIO_Pin_4);
         ADC_DCL();
				}break;
    //L DC-
    case 10:
	    if(uart3_receive==0xE2)GPIO_ResetBits(GPIOD, GPIO_Pin_5);
      else if(uart3_receive==0xE3)
				{
				 GPIO_SetBits(GPIOD, GPIO_Pin_5);
		     ADC_DCL();
				}break;
				
		//R 大电机R继电器	
		case 11:
			if(uart3_receive==0xC3)GPIO_SetBits(GPIOG, GPIO_Pin_4);
		  else if(uart3_receive==0xC2)GPIO_ResetBits(GPIOG, GPIO_Pin_4);break;
		
		//R DC+
	  case 12:
	    if(uart3_receive==0xE4)GPIO_ResetBits(GPIOG, GPIO_Pin_9);
      else if(uart3_receive==0xE5)
				{
				 GPIO_SetBits(GPIOG, GPIO_Pin_9);
					ADC_DCR();
				}break;
					
     //R DC-
    case 14:
	    if(uart3_receive==0xE6)GPIO_ResetBits(GPIOG, GPIO_Pin_10);
      else if(uart3_receive==0xE7)
				{
					GPIO_SetBits(GPIOG, GPIO_Pin_10);
					ADC_DCR();
					}break;
				
		//复位
		case 15:
		if(uart3_receive==0xF0)
			{	
		//推顶L
		GPIO_SetBits(GPIOG, GPIO_Pin_3);delay_ms(1000);GPIO_SetBits(GPIOG, GPIO_Pin_3);
		//推顶R
		GPIO_SetBits(GPIOD, GPIO_Pin_15);delay_ms(1000);GPIO_SetBits(GPIOD, GPIO_Pin_15);
    //小电机L
		if(ON_OFF1==0)GPIO_SetBits(GPIOC, GPIO_Pin_9);
				     else GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		//小电机R
		if(ON_OFF3==0)GPIO_SetBits(GPIOC, GPIO_Pin_6);
					   else GPIO_ResetBits(GPIOC, GPIO_Pin_6);

		//大电机L
		if(ON_OFF3==0)GPIO_SetBits(GPIOG, GPIO_Pin_5);
				     else GPIO_ResetBits(GPIOG, GPIO_Pin_5);		
		//大电机R
		if(ON_OFF4==0)GPIO_SetBits(GPIOG, GPIO_Pin_4);
				     else GPIO_ResetBits(GPIOG, GPIO_Pin_4);		
			}
		}
	}
}
#endif	

