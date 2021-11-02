#include "encode2.h"
#include "stm32f10x.h"
#include "misc.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
#include "RS485.h"
#include "Parse_frame_data.h"
#include "flag_function.h"

//�ڽǱ�����
u16 para_encode2 = 1000; //������2������
uint8_t Bencode[3];

void TIM2_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/*- ������������������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           

	/*- TIM2������ģʽ���� -*/
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = para_encode2 * 4;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);              
                 
//	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//���ñ�����ģʽ����Դ�ͼ���
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
	
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//�����˲���
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM2�ĸ��±�־λ
	
		/*��������������ж�*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//���и����ж�
		
	TIM2->CNT = 0;

	TIM_Cmd(TIM2, ENABLE);   //����TIM2��ʱ��
}

void TIM2_Init(void)
{
  TIM2_Mode_Config();
}

/*----------------------------------------------------End line-------------------------------------------------------------*/

u16 angle2=0,area2=0,ANGLE2=0;

//�ڽ� �˶�����dir2 λ��area2 �Ƕ�ANGLE2 
void encode2(void)
{
  static int dir2=0;
	dir2=(TIM2->CR1&0x0010)>4;//ȡ�˶������ַλ

	switch (dir2)
  {
  	case 0://��ʱ��
			if (dir2==0)
				{
					angle2=(int)((TIM2->CNT/4)*0.36);
					if (angle2>=270){area2=1;ANGLE2=360-angle2;}//λ���Ҳ�
					else {area2=0;ANGLE2=angle2;}//λ�����	
				}
				
				Bencode[0]=dir2;
				Bencode[1]=area2;
				Bencode[2]=ANGLE2; 
				
				break;
		
  	case 1://˳ʱ��
			if (dir2==1)
				{
					angle2=(int)(((4000-TIM2->CNT)/4)*0.36);
					if (angle2>=270){area2=0;ANGLE2=360-angle2;} //λ�����
					else {area2=1;ANGLE2=angle2;}//λ���Ҳ�
				}
				
				Bencode[0]=dir2;
				Bencode[1]=area2;
				Bencode[2]=ANGLE2; 
				
				break;
    	
    default:
    break;
    }
	//delay_ms(10);	
}

//�ڽ���λ
void encode2_limit(void)
	{
		/*�ڽ� -25~+25*/
		encode2();
		if(ANGLE2>=0X0F&&area2==1)GPIO_SetBits(GPIOG,GPIO_Pin_5);
		else if(ANGLE2>=0X0F&&area2==0)GPIO_SetBits(GPIOG,GPIO_Pin_4);
		//delay_ms(10);
	}


//���������ݷ��ͺ���
void encode2_Send_Data(void)
	{
		encode2();
		pack_protocol(2);
		//encode2_limit();
		RS485_Send_Data(RS485_TX_BUF,8);
		//delay_ms(10);
	}


//�ڽǸ�λ �ڽǽӽ�������λ PA4
void encode2_RESET(void)
	{
					encode2();
					
					if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0)//ȷ���ӽ�����״̬
						{
							if(area2==1)
								{GPIO_ResetBits(GPIOG,GPIO_Pin_4);GPIO_SetBits(GPIOG,GPIO_Pin_5);}//����� ����
							else if(area2==0)
                {GPIO_ResetBits(GPIOG,GPIO_Pin_5);GPIO_SetBits(GPIOG,GPIO_Pin_4);}//���Ҳ� ����
						  delay_ms(10);
						}
						else
						{
							GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5);
							TIM2_Init();
							delay_us(5);
							pack_protocol(9);
							RS485_Send_Data(RS485_TX_BUF,8);
							clear_resetfunction_flag();
						}
	}	
//void encode2_RESET(void)
//	{
//		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0)//�жϵ�ǰ״̬���Ƿ����ѭ��
//				{
//					encode2();
//					
//					if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0)//ȷ���ӽ�����״̬
//						{
//							if(area2==1)
//								{GPIO_ResetBits(GPIOG,GPIO_Pin_4);GPIO_SetBits(GPIOG,GPIO_Pin_5);}//����� ����
//							else if(area2==0)
//                {GPIO_ResetBits(GPIOG,GPIO_Pin_5);GPIO_SetBits(GPIOG,GPIO_Pin_4);}//���Ҳ� ����
//						  delay_ms(10);
//						}
//						else break;		
//				}
//				GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5);
//				TIM2_Init();
//				delay_us(5);
//				
//				pack_protocol(9);
//		    RS485_Send_Data(RS485_TX_BUF,8);
//	}
	
void 	user_Encode2_ResetL(void)
{
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0)
		{
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);GPIO_SetBits(GPIOG,GPIO_Pin_5);
		}
		GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5);
}

void 	user_Encode2_ResetR(void)
{
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0)
		{
			GPIO_ResetBits(GPIOG,GPIO_Pin_5);GPIO_SetBits(GPIOG,GPIO_Pin_4);
		}
		GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5);
}
