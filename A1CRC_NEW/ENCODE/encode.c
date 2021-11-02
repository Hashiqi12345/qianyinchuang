#include "stm32f10x.h"
#include "encode.h"
#include "misc.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"	
#include "Parse_frame_data.h"
#include "RS485.h"
#include "flag_function.h"

//��դ������
u16 gs_code= 6000; 
uint8_t Gencode[2];
	
void TIM4_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	
	
	//PB6 ch1  A,PB7 ch2 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ��TIM4ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOAʱ��
	
		/*- ������������������ PB->6  PB->7 -*/
	GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PB6 PB7��������	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

	/*- TIM1������ģʽ���� -*/
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);//����ȱʡֵ
	TIM_TimeBaseStructure.TIM_Period = gs_code*4;  //�趨��������װֵ   TIMx_ARR
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3ʱ��Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
  //TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//���ñ�����ģʽ����Դ�ͼ���

  TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM4
	
  //TIM_ARRPreloadConfig(TIM4, ENABLE);//ʹ��Ԥװ��
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM4�ĸ��±�־λ
	
	/*��������������ж�*/
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//���и����ж�

	TIM4->CNT = 0;

	TIM_Cmd(TIM4, ENABLE);   //����TIM4��ʱ��

}


void TIM4_Init(void)
{

  TIM4_Mode_Config();
}

/*----------------------------------------------------End line-------------------------------------------------------------*/

u8 distance;

//ǣ�� ����dir ����distance
void encode(void)
{
  static int dir;
	dir=(TIM4->CR1&0x0010)>4;
	distance=(int)((TIM4->CNT/4)/50);
	
//	Gencode[0]=dir;
Gencode[1]=distance;
//	printf("%d\r\n",distance);
	
	switch (dir)
  {
  	case 0://ǣ��
			if(dir==0)
				Gencode[0]=dir;
			  Gencode[1]=distance;
			printf("%d\r\n",distance);
  		break;
  	case 1://�ظ�
			if(dir==1)
				Gencode[0]=dir;
			  Gencode[1]=distance;
			   printf("%d\r\n",distance);
  		break;
			
			default:
    break;
}
}

//ǣ����λ
u8 mode;
void encode_limit(u8 mode)
	{
		switch (mode)
    {
    	case 0:
				/*ǣ�� 0~70mm*/
				encode();
				if(distance>=0X46)GPIO_SetBits(GPIOD,GPIO_Pin_14);
			  if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)==1){GPIO_SetBits(GPIOD,GPIO_Pin_15);TIM4_Init();}
				delay_ms(5);
    		break;
			
    	case 1:
				/*0~30mm*/
				encode();
				if(distance>=0X1E)GPIO_SetBits(GPIOD,GPIO_Pin_14);
			  if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)==1){GPIO_SetBits(GPIOD,GPIO_Pin_15);TIM4_Init();}
				delay_ms(5);
    		break;
			
    	default:
    		break;
    }
	}

//���������ݷ��ͺ���
void encode_Send_Data(void)
	{
		encode();
		pack_protocol(4);
		RS485_Send_Data(RS485_TX_BUF,8);
		delay_us(10);
	}	

//��դ�߸�λ ��е�г̿��ظ�λ PB1
void encode_RESET(void)
{
	if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)==0)//ȷ����е�г̿���״̬
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_15);//����� ����
			delay_ms(10);							
		}
		else
		{
			GPIO_SetBits(GPIOD,GPIO_Pin_15);//GPIO_Pin_14|
			TIM4_Init();
			pack_protocol(13);
			RS485_Send_Data(RS485_TX_BUF,8);
      clear_resetfunction_flag();			
		}							
}
	
//void encode_RESET(void)
//{
//			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)==0)//�жϵ�ǰ״̬���Ƿ����ѭ��
//				{
//					//encode();
//					
//					if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)==0)//ȷ����е�г̿���״̬
//						{
//							GPIO_ResetBits(GPIOD,GPIO_Pin_15);//����� ����
//							delay_ms(10);							
//						}
//						else break;	
//				}	
//				//delay_ms(10);
//				GPIO_SetBits(GPIOD,GPIO_Pin_15);//GPIO_Pin_14|
//        TIM4_Init();

//      pack_protocol(13);
//		  RS485_Send_Data(RS485_TX_BUF,8);				
//}

