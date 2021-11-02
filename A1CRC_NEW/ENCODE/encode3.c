#include "encode3.h"
#include "stm32f10x.h"
#include "misc.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
#include "RS485.h"
#include "Parse_frame_data.h"
#include "flag_function.h"

//���Ǳ�����
u16 para_encode3 = 1024; //������3������
uint8_t Xencode[3];

void TIM1_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO , ENABLE);//| RCC_APB2Periph_AFIO
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
	
	/*- ������������������  -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);                           
	
	/*- TIM1������ģʽ���� -*/
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = para_encode3 * 4;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);              
                 
//	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//���ñ�����ģʽ����Դ�ͼ���
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//�����˲���
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
		
  TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update );//���TIM1�ĸ��±�־λ
	
	/*��������������ж�*/
  TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//���и����ж�
	
	TIM1->CNT = 0;
		
	TIM_Cmd(TIM1, ENABLE);   //����TIM1��ʱ��
}

void TIM1_Init(void)
{
  TIM1_Mode_Config();
}

/*----------------------------------------------------End line-------------------------------------------------------------*/

u16 angle3=0,area3=0,ANGLE3=0;
//���� �˶�����dir3 λ��area3 �Ƕ�ANGLE3

void encode3(void)
{
  static int dir3=0;
	dir3=(TIM1->CR1&0x0010)>4;

	switch (dir3)
  {
  	case 0://��ʱ��
			if (dir3==0)
				{
					angle3=(int)((TIM1->CNT/4)*0.35);
					if (angle3>=270){area3=1;ANGLE3=360-angle3;}//λ���Ҳ�
					else {area3=0;ANGLE3=angle3;}	//λ�����
				}
				
				Xencode[0]=dir3;
				Xencode[1]=area3;
				Xencode[2]=ANGLE3; 
				
				break;
		
  	case 1://˳ʱ��
			if (dir3==1)
				{
					angle3=(int)(((4096-TIM1->CNT)/4)*0.35);
					if (angle3>=270){area3=0;ANGLE3=360-angle3;}//λ�����
					else {area3=1;ANGLE3=angle3;}	//λ���Ҳ�
				}
				
				Xencode[0]=dir3;
				Xencode[1]=area3;
				Xencode[2]=ANGLE3; 
				
				break;
    	
    default:
    break;
    }	
	//delay_ms(10);
}

//������λ
void encode3_limit(void)
	{
		/*���� -25~+25*/
		encode3();
		if(ANGLE3>=0X0B&&area3==0)GPIO_SetBits(GPIOG,GPIO_Pin_2);
		else if(ANGLE3>=0X0B&&area3==1)GPIO_SetBits(GPIOG,GPIO_Pin_3);
		//delay_ms(10);	
	
	}

//���������ݷ��ͺ���
void encode3_Send_Data(void)
	{
		encode3();
		pack_protocol(3);
    //encode3_limit();
		RS485_Send_Data(RS485_TX_BUF,8);
		//delay_ms(10);
	}

//���Ǹ�λ ���ǽӽ�������λ PB0
void encode3_RESET(void)
{
	encode3();
	if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)==0)//ȷ���ӽ�����״̬
		{
			if(area3==1)
				{GPIO_ResetBits(GPIOG,GPIO_Pin_2);GPIO_SetBits(GPIOG,GPIO_Pin_3);}//����� ����
			else if(area3==0)
				{GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2);}//���Ҳ� ����
			delay_ms(10);
		}
		else
		{
			GPIO_SetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3);
			TIM1_Init();
			delay_us(5);
			pack_protocol(11);
			RS485_Send_Data(RS485_TX_BUF,8);
			clear_resetfunction_flag();						
		}
	}	
//void encode3_RESET(void)
//{
//	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)==0)//�жϵ�ǰ״̬���Ƿ����ѭ��
//			{
//				encode3();
//				if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)==0)//ȷ���ӽ�����״̬
//					{
//						if(area3==1)
//							{GPIO_ResetBits(GPIOG,GPIO_Pin_2);GPIO_SetBits(GPIOG,GPIO_Pin_3);}//����� ����
//						else if(area3==0)
//							{GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2);}//���Ҳ� ����
//						delay_ms(10);
//					}
//					else break;		
//			}
//			GPIO_SetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3);
//			TIM1_Init();
//			delay_us(5);
//			
//			pack_protocol(11);
//			RS485_Send_Data(RS485_TX_BUF,8); 	
//}

	