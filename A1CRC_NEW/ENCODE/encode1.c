#include "encode1.h"
#include "stm32f10x.h"
#include "misc.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"	
#include "Parse_frame_data.h"
#include "RS485.h"
#include "LED.h"
#include "flag_function.h"

 //�ɽǱ�����
 u16 para_encode1=1000;//������1������
 uint8_t Cencode[3];

  void TIM3_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/*- ������������������ PA->6   PA->7 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           
  
	//NVIC_Config(2);
	/*- TIM4������ģʽ���� -*/
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = para_encode1*4;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
                 
//	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//���ñ�����ģʽ����Դ�ͼ���
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
	
	TIM_ICStructInit(&TIM_ICInitStructure);	//���ṹ���е�����ȱʡ����																																	//�����˲���
	TIM_ICInitStructure.TIM_ICFilter = 6;//ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��
		
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM3�ĸ��±�־λ	
		
	/*��������������ж�*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//���и����ж�

	TIM3->CNT = 0;

	TIM_Cmd(TIM3, ENABLE);   //����TIM3��ʱ��
}

void TIM3_Init(void)
{
  TIM3_Mode_Config();
}

/*----------------------------------------------------End line-------------------------------------------------------------*/

u16 angle1,area1,ANGLE1;

//�ɽ� �˶�����dir1 λ��area1 �Ƕ�ANGLE1 
void encode1(void)
{
  static int dir1=0;
	dir1=(TIM3->CR1&0x0010)>4;
	
	switch (dir1)
  {
  	case 0://��ʱ��
			if (dir1==0)
				{
					angle1=(TIM3->CNT/4)*0.36;
					if (angle1>=270){area1=1;ANGLE1=360-angle1;}//printf ("dir1=%d,area1=%d,angle1=%d\r\n",dir1,area1,ANGLE1);}//λ���²�
					else {area1=0;ANGLE1=angle1;}//printf ("dir1=%d,area1=%d,angle1=%d\r\n",dir1,area1,ANGLE1);}		//λ���ϲ�
				}
				
				Cencode[0]=dir1;
				Cencode[1]=area1;
				Cencode[2]=ANGLE1; 
				
				break;
		
  	case 1://˳ʱ��
			if (dir1==1)
				{
					angle1=(4000-TIM3->CNT)/4*0.36;
					if (angle1>=270){area1=0;ANGLE1=360-angle1;}//printf ("dir1=%d,area1=%d,angle1=%d\r\n",dir1,area1,ANGLE1);}//λ���ϲ�
					else {area1=1;ANGLE1=angle1;}//printf ("dir1=%d,area1=%d,angle1=%d\r\n",dir1,area1,ANGLE1);}		//λ���²�
				}
				
				Cencode[0]=dir1;
				Cencode[1]=area1;
				Cencode[2]=ANGLE1; 
				
				break;
    	
    default:
    break;
    }
	//delay_ms(10);//printf ("dir1=%d,area1=%d,angle1=%d\r\n",dir1,area1,ANGLE1);
	}


	//�ɽ���λ
void encode1_limit(void)
	{
			/*�ɽ� -5~+25*/
		encode1();
		if(area1==0&&ANGLE1>=0X0B)GPIO_SetBits(GPIOG,GPIO_Pin_6);
		else if(area1==1&&ANGLE1>=0X03)GPIO_SetBits(GPIOG,GPIO_Pin_7);
			//delay_ms(10);
	}	
	
	
//���������ݷ��ͺ���	
void encode1_Send_Data(void)
	{
		encode1();
		pack_protocol(1);
		//encode1_limit();
		RS485_Send_Data(RS485_TX_BUF,8);
		//delay_ms(10);
	}	
	
	
//�ɽǸ�λ �ɽǹ�����λ PA5
void encode1_RESET(void)
{
	encode1();
	if(Cencode[2]!=0)//ȷ���ӽ�����״̬
		{
			if(area1==0)
				{GPIO_ResetBits(GPIOG,GPIO_Pin_7);GPIO_SetBits(GPIOG,GPIO_Pin_6);}//���ϲ� ����
			else if(area1==1)
				{GPIO_ResetBits(GPIOG,GPIO_Pin_6);GPIO_SetBits(GPIOG,GPIO_Pin_7);}//���²� ����
			delay_ms(10);
		}
	else
		{
			GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7);
			TIM3_Init();			
			delay_us(5);	
			pack_protocol(7);
		  RS485_Send_Data(RS485_TX_BUF,8);
			clear_resetfunction_flag();
		}	
}		
//void encode1_RESET(void)
//	{
//			while(Cencode[2]!=0)//�жϵ�ǰ״̬���Ƿ����ѭ��
//				{
//					 encode1();
//					if(Cencode[2]!=0)//ȷ���ӽ�����״̬
//						{
//							if(area1==0)
//								{GPIO_ResetBits(GPIOG,GPIO_Pin_7);GPIO_SetBits(GPIOG,GPIO_Pin_6);}//���ϲ� ����
//							else if(area1==1)
//								{GPIO_ResetBits(GPIOG,GPIO_Pin_6);GPIO_SetBits(GPIOG,GPIO_Pin_7);}//���²� ����
//						  delay_ms(10);
//						}
//						else break;		
//				}
//				GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7);
//				TIM3_Init();
//				delay_us(5);
//				
//				 pack_protocol(7);
//		  RS485_Send_Data(RS485_TX_BUF,8);
//			}				
/*************
u8 Count1,angle1;
void TIM3_IRQHandler(void)
{
	Count1 = TIM3->CNT/4;														  //��ȡ������1��ǰ��ֵ
	angle1 = Count1*0.36;
	printf("angl1 = %d\n\r\n",angle1);
	delay_ms(1000);	
}
***************/


