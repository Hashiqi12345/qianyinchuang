#include "stm32f10x.h"
#include "USART3.h"
#include "sys.h" 
#include "ON_OFF.h"
#include "2803.h"  
#include "Debug_PIN.h"
#include "adc.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 



//�ô��������봥����ͨѶ


//���ʹ��ucos,����������ͷ�ļ�����.

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

//FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
//�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{      
	//while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
  // USART3->DR = (u8) ch;      
//return ch;
//}
#endif 

/*ʹ��microLib�ķ���*/
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
 
#if EN_USART3_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	  
  
void uart3_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	
	//USART3_TX   GPIOB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB10
   
  //USARTB_RX	  GPIOB11��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB11 

  //Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 

}

//���ڽ����ɴ�����������ָ�uart3_receive���յ��ĵ�ַΪ�������������Ӧ��ַ
void USART3_IRQHandler(void)                	//����3�жϷ������
	{
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�3
		{
			static	int uart3_receive=0;
			u8 i=0;
		  uart3_receive =USART_ReceiveData(USART3);	//��ȡ���յ�������
			switch(i)
		{  
    //LU ����L->��ŷ�L1
	  case 0:
			if(ON_OFF3==0)
				{
					if(uart3_receive==0xA1)GPIO_SetBits(GPIOG, GPIO_Pin_2);
					else if(uart3_receive==0xA0)GPIO_ResetBits(GPIOG, GPIO_Pin_2);	
				}
				else GPIO_ResetBits(GPIOG, GPIO_Pin_2);	
				break;

	  //LD ����L->��ŷ�L2				
		case 1:
			if(uart3_receive==0xA3)GPIO_SetBits(GPIOG, GPIO_Pin_3);
      else if(uart3_receive==0xA2)GPIO_ResetBits(GPIOG, GPIO_Pin_3);break;

    //RU ����R->��ŷ�R1		
	  case 2:
			if(ON_OFF3==0)
				{
					if(uart3_receive==0xA5)GPIO_SetBits(GPIOD, GPIO_Pin_14);
					else if(uart3_receive==0xA4)GPIO_ResetBits(GPIOD, GPIO_Pin_14);	
				}
				else GPIO_ResetBits(GPIOD, GPIO_Pin_14);	break;

	  //RD ����R->��ŷ�L2					
		case 3:
			if(uart3_receive==0xA7)GPIO_SetBits(GPIOD, GPIO_Pin_15);
      else if(uart3_receive==0xA6)GPIO_ResetBits(GPIOD, GPIO_Pin_15);			break;
			
	  //LL ����L->С���L ��			   	
		case 4:
			if(ON_OFF1==0)
				{
					if(uart3_receive==0xB1)GPIO_SetBits(GPIOC, GPIO_Pin_9);
					else if(uart3_receive==0xB0)GPIO_ResetBits(GPIOC, GPIO_Pin_9);
				}
				else GPIO_ResetBits(GPIOC, GPIO_Pin_9);break;
				
		//LR ����L->С���L ��			
	  case 5:
		  if(uart3_receive==0xB3)GPIO_SetBits(GPIOC, GPIO_Pin_8);
	    else if(uart3_receive==0xB2)GPIO_ResetBits(GPIOC, GPIO_Pin_8);break;
		
		//RL ����R->С���R ��			   	
		case 6:
			if(ON_OFF2==0)
				{
					if(uart3_receive==0xB5)GPIO_SetBits(GPIOC, GPIO_Pin_6);
					else if(uart3_receive==0xB4)GPIO_ResetBits(GPIOC, GPIO_Pin_6);
				}
				else GPIO_ResetBits(GPIOC, GPIO_Pin_6);break;
				
		//RR ����R->С���R ��			
	  case 7:
		  if(uart3_receive==0xB7)GPIO_SetBits(GPIOC, GPIO_Pin_7);
	    else if(uart3_receive==0xB6)GPIO_ResetBits(GPIOC, GPIO_Pin_7);break;
		
		//L ����L�̵���	
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
				
		//R ����R�̵���	
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
				
		//��λ
		case 15:
		if(uart3_receive==0xF0)
			{	
		//�ƶ�L
		GPIO_SetBits(GPIOG, GPIO_Pin_3);delay_ms(1000);GPIO_SetBits(GPIOG, GPIO_Pin_3);
		//�ƶ�R
		GPIO_SetBits(GPIOD, GPIO_Pin_15);delay_ms(1000);GPIO_SetBits(GPIOD, GPIO_Pin_15);
    //С���L
		if(ON_OFF1==0)GPIO_SetBits(GPIOC, GPIO_Pin_9);
				     else GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		//С���R
		if(ON_OFF3==0)GPIO_SetBits(GPIOC, GPIO_Pin_6);
					   else GPIO_ResetBits(GPIOC, GPIO_Pin_6);

		//����L
		if(ON_OFF3==0)GPIO_SetBits(GPIOG, GPIO_Pin_5);
				     else GPIO_ResetBits(GPIOG, GPIO_Pin_5);		
		//����R
		if(ON_OFF4==0)GPIO_SetBits(GPIOG, GPIO_Pin_4);
				     else GPIO_ResetBits(GPIOG, GPIO_Pin_4);		
			}
		}
	}
}
#endif	

