#include "encode3.h"
#include "stm32f10x.h"
#include "misc.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
#include "RS485.h"
#include "Parse_frame_data.h"
#include "flag_function.h"

//旋角编码器
u16 para_encode3 = 1024; //编码器3的线数
uint8_t Xencode[3];

void TIM1_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO , ENABLE);//| RCC_APB2Periph_AFIO
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
	
	/*- 正交编码器输入引脚  -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);                           
	
	/*- TIM1编码器模式配置 -*/
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = para_encode3 * 4;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);              
                 
//	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
		
  TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update );//清除TIM1的更新标志位
	
	/*开启编码器溢出中断*/
  TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//运行更新中断
	
	TIM1->CNT = 0;
		
	TIM_Cmd(TIM1, ENABLE);   //启动TIM1定时器
}

void TIM1_Init(void)
{
  TIM1_Mode_Config();
}

/*----------------------------------------------------End line-------------------------------------------------------------*/

u16 angle3=0,area3=0,ANGLE3=0;
//旋角 运动方向dir3 位置area3 角度ANGLE3

void encode3(void)
{
  static int dir3=0;
	dir3=(TIM1->CR1&0x0010)>4;

	switch (dir3)
  {
  	case 0://逆时针
			if (dir3==0)
				{
					angle3=(int)((TIM1->CNT/4)*0.35);
					if (angle3>=270){area3=1;ANGLE3=360-angle3;}//位于右侧
					else {area3=0;ANGLE3=angle3;}	//位于左侧
				}
				
				Xencode[0]=dir3;
				Xencode[1]=area3;
				Xencode[2]=ANGLE3; 
				
				break;
		
  	case 1://顺时针
			if (dir3==1)
				{
					angle3=(int)(((4096-TIM1->CNT)/4)*0.35);
					if (angle3>=270){area3=0;ANGLE3=360-angle3;}//位于左侧
					else {area3=1;ANGLE3=angle3;}	//位于右侧
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

//旋角限位
void encode3_limit(void)
	{
		/*旋角 -25~+25*/
		encode3();
		if(ANGLE3>=0X0B&&area3==0)GPIO_SetBits(GPIOG,GPIO_Pin_2);
		else if(ANGLE3>=0X0B&&area3==1)GPIO_SetBits(GPIOG,GPIO_Pin_3);
		//delay_ms(10);	
	
	}

//编码器数据发送函数
void encode3_Send_Data(void)
	{
		encode3();
		pack_protocol(3);
    //encode3_limit();
		RS485_Send_Data(RS485_TX_BUF,8);
		//delay_ms(10);
	}

//旋角复位 旋角接近开关限位 PB0
void encode3_RESET(void)
{
	encode3();
	if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)==0)//确定接近开关状态
		{
			if(area3==1)
				{GPIO_ResetBits(GPIOG,GPIO_Pin_2);GPIO_SetBits(GPIOG,GPIO_Pin_3);}//在左侧 向右
			else if(area3==0)
				{GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2);}//在右侧 向左
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
//	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)==0)//判断当前状态，是否进入循环
//			{
//				encode3();
//				if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)==0)//确定接近开关状态
//					{
//						if(area3==1)
//							{GPIO_ResetBits(GPIOG,GPIO_Pin_2);GPIO_SetBits(GPIOG,GPIO_Pin_3);}//在左侧 向右
//						else if(area3==0)
//							{GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2);}//在右侧 向左
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

	