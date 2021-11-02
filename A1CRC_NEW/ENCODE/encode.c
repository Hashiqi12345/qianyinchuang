#include "stm32f10x.h"
#include "encode.h"
#include "misc.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"	
#include "Parse_frame_data.h"
#include "RS485.h"
#include "flag_function.h"

//光栅编码器
u16 gs_code= 6000; 
uint8_t Gencode[2];
	
void TIM4_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	
	
	//PB6 ch1  A,PB7 ch2 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能TIM4时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOA时钟
	
		/*- 正交编码器输入引脚 PB->6  PB->7 -*/
	GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PB6 PB7浮空输入	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

	/*- TIM1编码器模式配置 -*/
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);//设置缺省值
	TIM_TimeBaseStructure.TIM_Period = gs_code*4;  //设定计数器重装值   TIMx_ARR
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3时钟预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
  //TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性

  TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 6;  //选择输入比较滤波器 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM4
	
  //TIM_ARRPreloadConfig(TIM4, ENABLE);//使能预装载
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM4的更新标志位
	
	/*开启编码器溢出中断*/
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//运行更新中断

	TIM4->CNT = 0;

	TIM_Cmd(TIM4, ENABLE);   //启动TIM4定时器

}


void TIM4_Init(void)
{

  TIM4_Mode_Config();
}

/*----------------------------------------------------End line-------------------------------------------------------------*/

u8 distance;

//牵引 方向dir 距离distance
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
  	case 0://牵引
			if(dir==0)
				Gencode[0]=dir;
			  Gencode[1]=distance;
			printf("%d\r\n",distance);
  		break;
  	case 1://回复
			if(dir==1)
				Gencode[0]=dir;
			  Gencode[1]=distance;
			   printf("%d\r\n",distance);
  		break;
			
			default:
    break;
}
}

//牵引限位
u8 mode;
void encode_limit(u8 mode)
	{
		switch (mode)
    {
    	case 0:
				/*牵引 0~70mm*/
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

//编码器数据发送函数
void encode_Send_Data(void)
	{
		encode();
		pack_protocol(4);
		RS485_Send_Data(RS485_TX_BUF,8);
		delay_us(10);
	}	

//光栅尺复位 机械行程开关复位 PB1
void encode_RESET(void)
{
	if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)==0)//确定机械行程开关状态
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_15);//在左侧 向右
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
//			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)==0)//判断当前状态，是否进入循环
//				{
//					//encode();
//					
//					if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)==0)//确定机械行程开关状态
//						{
//							GPIO_ResetBits(GPIOD,GPIO_Pin_15);//在左侧 向右
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

