#include "function.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "port.h"
#include "LED.h"	
#include "encode.h"
#include "encode1.h"
#include "encode2.h"
#include "encode3.h"
#include "adc.h"
#include "usart.h"	
#include "Parse_frame_data.h"
#include "RS485.h"
#include "Execute_function.h"
#include "stmflash.h"
#include "flag_function.h"
/***********************
【函 数 名】：DISTANCE_function
【参数说明】: 
***********************/
//牵引距离DISTANCE

void DISTANCE_function(u8 num)
{
	switch (num)
  {
		//牵引
  	case 0:
			GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		  GPIO_SetBits(GPIOD,GPIO_Pin_15);
//		  encode();
		  encode_limit(mode);
		break;
		
		//回复
		case 1:
			GPIO_ResetBits(GPIOD,GPIO_Pin_15);
		  GPIO_SetBits(GPIOD,GPIO_Pin_14);
//		  encode();
		  encode_limit(mode);
		break;
		
  	case 2://停止
			GPIO_SetBits(GPIOD,GPIO_Pin_14|GPIO_Pin_15);
			pack_protocol(14);
			RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
		  encode_Send_Data();
		break;
		
		case 3:encode();break;
		
		case 4://牵引复位
			encode_RESET();
			delay_us(5);break;
		
		default:break;
  }
}


/***********************
【函 数 名】：ANGLE1_function
【参数说明】: 
***********************/
//成角ANGLE1

void ANGLE1_function(u8 num1)
{ 
	switch (num1)
  {
		//向上运动
  	case 0:
			GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		  GPIO_SetBits(GPIOG,GPIO_Pin_6);
		  //encode1();
		  encode1_limit();
		break;//成角电机启动
		//向下运动
		case 1:
			GPIO_ResetBits(GPIOG,GPIO_Pin_6);
		  GPIO_SetBits(GPIOG,GPIO_Pin_7);
		  //encode1();
		  encode1_limit();
		break;//成角电机启动
		
  	case 2:
			GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7);
		  pack_protocol(8);
		  RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
		  encode1_Send_Data();
		break;//成角电机停止
		
    case 3:encode1();break;
		case 4://成角复位
			encode1_RESET();
		 
			delay_us(5);
		break;
		
		default:break;
	}
}

/***********************
【函 数 名】：ANGLE2_function
【参数说明】: 
***********************/
//摆角ANGLE2

void ANGLE2_function(u8 num2)
{
	switch (num2)
  {
		//向左运动
  	case 0:
			GPIO_ResetBits(GPIOG,GPIO_Pin_5);
		  GPIO_SetBits(GPIOG,GPIO_Pin_4);
		  //encode2();
		  encode2_limit();
		break;//启动
		//向右运动
		case 1:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);
		  GPIO_SetBits(GPIOG,GPIO_Pin_5);
		  encode2();
		  encode2_limit();
		break;//启动
		
  	case 2:
			GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5);
		  pack_protocol(10);
		  RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
		  encode2_Send_Data();
		break;//停止
		
    case 3:encode2();break;
		case 4:
			encode2_RESET();
		 // pack_protocol(9);
		  //RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
		break;
		
		default:break;
	}
}

/***********************
【函 数 名】：ANGLE3_function
【参数说明】: 
***********************/
//旋角ANGLE3

void ANGLE3_function(u8 num3)
{ 
	switch (num3)
  {
		//正
  	case 0:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);
		  GPIO_SetBits(GPIOG,GPIO_Pin_2);
		 // encode3();
		  encode3_limit();
		break;//启动
		//反
		case 1:
			GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		  GPIO_SetBits(GPIOG,GPIO_Pin_3);
		 // encode3();
		  encode3_limit();
		break;//启动
		
  	case 2:
			GPIO_SetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3);
      pack_protocol(12);
		  RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
      encode3_Send_Data();		
		break;//停止
    case 3:encode3();break;
		case 4://旋角复位
			encode3_RESET();
		break;
		
		default:break;  
	}
}

/***********************
【函 数 名】：STOP急停
【参数说明】: 
***********************/
void STOP(void)
	{
		DISTANCE_function(2);encode_Send_Data();delay_us(5);
		ANGLE1_function(2);encode1_Send_Data();delay_us(5);
		ANGLE2_function(2);encode2_Send_Data();delay_us(5);
		ANGLE3_function(2);encode3_Send_Data();delay_us(5);
}
/***********************
【函 数 名】：RESET复位
【参数说明】: 
***********************/
void RESET_PLACE(void)
	{
	 encode_RESET();encode_Send_Data();
	 encode1_RESET();encode1_Send_Data();
	 encode2_RESET();encode2_Send_Data();
	 encode3_RESET();encode3_Send_Data();
	}

/***********************
根据上位机设定参数，执行功能
*光栅尺
*成角编码器
*摆角编码器
*旋角编码器
***********************/
extern u8 PROTOCOL[64];
	
void ENCODE_function(u8 num)
	{
		u8 DATA1,DATA2;
		u8 encode_temp_DATA;
		u8 encode1_temp_DATA1,encode1_temp_DATA2;
		u8 encode2_temp_DATA1,encode2_temp_DATA2;
		u8 encode3_temp_DATA1,encode3_temp_DATA2;
		
		DATA1=PROTOCOL[4];//获取上位机设置的区域
		DATA2=PROTOCOL[5];//获取上位机设置的位移、角度数据
		
		delay_us(5);
		
		switch (num)
			{
			  case 0://成角编码器
					encode1();
				  encode1_temp_DATA1=Cencode[1];//当前区域
				  encode1_temp_DATA2=Cencode[2];//当前角度
				
				  if((DATA1==0&&DATA2>=0x0B)||(DATA1==1&&DATA2>=0x03))break;
					
				  if(DATA1!=encode1_temp_DATA1||DATA2!=encode1_temp_DATA2)
						{
							encode1();
				      encode1_temp_DATA1=Cencode[1];//实际区域
				      encode1_temp_DATA2=Cencode[2];//实际角度
							
						if((encode1_temp_DATA1==0&&encode1_temp_DATA2<=0x0B)
							||(encode1_temp_DATA1==1&&encode1_temp_DATA2<=0x03))//限位
						{
							if(DATA1!=encode1_temp_DATA1&&DATA2!=encode1_temp_DATA2)//不同区域 不同角度
								{
									if(DATA1==0)ANGLE1_function(1);
									else if(DATA1==1)ANGLE1_function(0);
								}
								
							else if(DATA1==0&&DATA1==encode1_temp_DATA1&&DATA2!=encode1_temp_DATA2)//同上部 不同角度
								{
									if(DATA2>encode1_temp_DATA2)ANGLE1_function(1);
									else if(DATA2<encode1_temp_DATA2)ANGLE1_function(0);
								}
								
							else if(DATA1==1&&DATA1==encode1_temp_DATA1&&DATA2!=encode1_temp_DATA2)//同下部 不同角度
								{
									if(DATA2>encode1_temp_DATA2)ANGLE1_function(0);
									else if(DATA2<encode1_temp_DATA2)ANGLE1_function(1);
								}
						}
					} 
						else if(DATA1==encode1_temp_DATA1&&DATA2==encode1_temp_DATA2)//同区域 同角度
							{
								ANGLE1_function(2);
								clear_setdatafunction_flag();
								DATA1=0;DATA2=0;
							}	 	
      		break;
				
      	case 1://摆角编码器
					encode2();
				  encode2_temp_DATA1=Bencode[1];//当前区域
				  encode2_temp_DATA2=Bencode[2];//当前角度
				
				  if(DATA2>=0X0F)break;
				
					if(DATA1!=encode2_temp_DATA1||DATA2!=encode2_temp_DATA2)
					{
							encode2();
				      encode2_temp_DATA1=Bencode[1];//实际区域
				      encode2_temp_DATA2=Bencode[2];//实际角度
							
						if(encode2_temp_DATA2<0X0F)
						{
							if(DATA1!=encode2_temp_DATA1&&DATA2!=encode2_temp_DATA2)//不同区域 不同角度
								{
									if(DATA1==0)ANGLE2_function(1);
									else if(DATA1==1)ANGLE2_function(0);
								}
								
							else if(DATA1==0&&DATA1==encode2_temp_DATA1&&DATA2!=encode2_temp_DATA2)//同左侧 不同角度
								{
									if(DATA2>encode2_temp_DATA2)ANGLE2_function(1);
									else if(DATA2<encode2_temp_DATA2)ANGLE2_function(0);
								}
								
							else if(DATA1==1&&DATA1==encode2_temp_DATA1&&DATA2!=encode2_temp_DATA2)//同右侧 不同角度
								{
									if(DATA2>encode2_temp_DATA2)ANGLE2_function(0);
									else if(DATA2<encode2_temp_DATA2)ANGLE2_function(1);
								}
						}
					}
					else if(DATA1==encode2_temp_DATA1&&DATA2==encode2_temp_DATA2)//同区域 同角度
					{
						ANGLE2_function(2);
						delay_us(5);
						clear_setdatafunction_flag();
						DATA1=0;DATA2=0;
					}
      		break;
				
        case 2://旋角编码器
					encode3();
				  encode3_temp_DATA1=Xencode[1];//当前区域
				  encode3_temp_DATA2=Xencode[2];//当前角度
				
				  if(DATA2>=0X0B)break;
				
					if(DATA1!=encode3_temp_DATA1||DATA2!=encode3_temp_DATA2)
					{
							encode3();
				      encode3_temp_DATA1=Xencode[1];//实际区域
				      encode3_temp_DATA2=Xencode[2];//实际角度
							
						if(encode3_temp_DATA2<0X0A)
						{
							if(DATA1!=encode3_temp_DATA1&&DATA2!=encode3_temp_DATA2)//不同区域 不同角度
								{
									if(DATA1==0)ANGLE3_function(1);
									else if(DATA1==1)ANGLE3_function(0);
								}
								
							else if(DATA1==0&&DATA1==encode3_temp_DATA1&&DATA2!=encode3_temp_DATA2)//同左侧 不同角度
								{
									if(DATA2>encode3_temp_DATA2)ANGLE3_function(1);
									else if(DATA2<encode3_temp_DATA2)ANGLE3_function(0);
								}
								
							else if(DATA1==1&&DATA1==encode3_temp_DATA1&&DATA2!=encode3_temp_DATA2)//同右侧 不同角度
								{
									if(DATA2>encode3_temp_DATA2)ANGLE3_function(0);
									else if(DATA2<encode3_temp_DATA2)ANGLE3_function(1);
								}
						}
					}
					else if(DATA1==encode3_temp_DATA1&&DATA2==encode3_temp_DATA2)//同区域 同角度
						{
							ANGLE3_function(2);
							delay_us(5);
							clear_setdatafunction_flag();
							DATA1=0;DATA2=0;
						}
      		break;
				
        case 3://光栅尺  没有使用该选项
					encode();
				  encode_temp_DATA=distance;;//当前距离
				
	//			  if(DATA2>0X46)break;
				
					if(DATA2!=encode_temp_DATA)
						{
							encode();
				      encode_temp_DATA=distance;//实际距离
							
							if(encode_temp_DATA<=0X46)
							{
								if(DATA2>encode_temp_DATA)DISTANCE_function(0);
								else if(DATA2<encode_temp_DATA)DISTANCE_function(1);
							}		
						}
					else if(DATA2==encode_temp_DATA)
						{
							DISTANCE_function(2);
							delay_us(5);
							clear_setdatafunction_flag();
							DATA1=0;DATA2=0;
						}
      		break;	
				
      	default:
      		break;
      }
		}
	

void Baijiao_function(void)
{
	u8 BDATA1,BDATA2;	
	u8 encode2_temp_DATA1,encode2_temp_DATA2;
	
	BDATA1=PROTOCOL[4];//获取上位机设置的区域
	BDATA2=PROTOCOL[5];//获取上位机设置的位移、角度数据
	
	encode2();
	encode2_temp_DATA1=Bencode[1];//当前区域
	encode2_temp_DATA2=Bencode[2];//当前角度

	if(BDATA2>=0X0F){clear_setdatafunction_flag();}

	while(BDATA1!=encode2_temp_DATA1||BDATA2!=encode2_temp_DATA2)
		{
			if(BREAK_FLAG ==1 )break;//外部给赋值1，可强行跳出中断
			
			encode2();
			encode2_temp_DATA1=Bencode[1];//实际区域
			encode2_temp_DATA2=Bencode[2];//实际角度
			
		if(encode2_temp_DATA2<0X0F)
		{
			if(BDATA1!=encode2_temp_DATA1&&BDATA2!=encode2_temp_DATA2)//不同区域 不同角度
				{
					if(BDATA1==0)ANGLE2_function(1);
					else if(BDATA1==1)ANGLE2_function(0);
				}
				
			else if(BDATA1==0&&BDATA1==encode2_temp_DATA1&&BDATA2!=encode2_temp_DATA2)//同左侧 不同角度
				{
					if(BDATA2>encode2_temp_DATA2)ANGLE2_function(1);
					else if(BDATA2<encode2_temp_DATA2)ANGLE2_function(0);
				}
				
			else if(BDATA1==1&&BDATA1==encode2_temp_DATA1&&BDATA2!=encode2_temp_DATA2)//同右侧 不同角度
				{
					if(BDATA2>encode2_temp_DATA2)ANGLE2_function(0);
					else if(BDATA2<encode2_temp_DATA2)ANGLE2_function(1);
				}
				
			else if(BDATA1==encode2_temp_DATA1&&BDATA2==encode2_temp_DATA2)break;//同区域 同角度
		}
		else break;
		}
		
		ANGLE2_function(2);
		delay_us(5);
    clear_setdatafunction_flag();		

}

void Xuanjiao_function(void)
{
	u8 XDATA1,XDATA2;	
	u8 encode3_temp_DATA1,encode3_temp_DATA2;
	
	XDATA1=PROTOCOL[4];//获取上位机设置的区域
	XDATA2=PROTOCOL[5];//获取上位机设置的位移、角度数据
	
	encode3();
	encode3_temp_DATA1=Cencode[1];//当前区域
	encode3_temp_DATA2=Cencode[2];//当前角度

	if(XDATA2>=0X0B){clear_setdatafunction_flag();}

	while(XDATA1!=encode3_temp_DATA1||XDATA2!=encode3_temp_DATA2)
		{
			if(BREAK_FLAG == 1)break;//外部给赋值1，可强行跳出中断
			
			encode3();
			encode3_temp_DATA1=Xencode[1];//实际区域
			encode3_temp_DATA2=Xencode[2];//实际角度
			
		if(encode3_temp_DATA2<0X0B)
		{
			if(XDATA1!=encode3_temp_DATA1&&XDATA2!=encode3_temp_DATA2)//不同区域 不同角度
				{
					if(XDATA1==0)ANGLE3_function(1);
					else if(XDATA1==1)ANGLE3_function(0);
				}
				
			else if(XDATA1==0&&XDATA1==encode3_temp_DATA1&&XDATA2!=encode3_temp_DATA2)//同左侧 不同角度
				{
					if(XDATA2>encode3_temp_DATA2)ANGLE3_function(1);
					else if(XDATA2<encode3_temp_DATA2)ANGLE3_function(0);
				}
				
			else if(XDATA1==1&&XDATA1==encode3_temp_DATA1&&XDATA2!=encode3_temp_DATA2)//同右侧 不同角度
				{
					if(XDATA2>encode3_temp_DATA2)ANGLE3_function(0);
					else if(XDATA2<encode3_temp_DATA2)ANGLE3_function(1);
				}
				
			else if(XDATA1==encode3_temp_DATA1&&XDATA2==encode3_temp_DATA2)break;//同区域 同角度
		}
		else break;
		}
		
		ANGLE3_function(2);
		delay_us(5);
		clear_setdatafunction_flag();
}

void QianYin_function(void)
{
	u8 encode_temp_DATA;
  u8 GDATA;	
	
	GDATA=PROTOCOL[5];//获取上位机设置的位移、角度数据
	encode();
	encode_temp_DATA=Gencode[1];;//当前距离

  if(GDATA>=0X46){clear_setdatafunction_flag();}
				
	while(GDATA!=encode_temp_DATA)
		{	
			if(BREAK_FLAG == 1)break;//外部给赋值1，可强行跳出中断		
			
			encode();
			encode_temp_DATA=Gencode[1];//实际距离
			
			if(encode_temp_DATA<0X46)
			{
				if(GDATA>encode_temp_DATA)DISTANCE_function(0);
				else if(GDATA<encode_temp_DATA)DISTANCE_function(1);
				else if(GDATA==encode_temp_DATA)break;
			}
			else break;
		}
		
		DISTANCE_function(2);
		delay_us(5);
    clear_setdatafunction_flag();		
}
	
///***********************
//【函 数 名】：AngleAreaData_Save
//【参数说明】: 将角度变化的位置区域信息存在FLASH中
//***********************/	
//u16 AngleAreaData[3];	
//#define SIZE sizeof(AngleAreaData)		//数组长度		
//#define FLASH_SAVE_ADDR  0X08070000

//u16 datatemp[SIZE];		

//void AngleAreaData_Save(void)
//	{	
//		//u8 i;
//		
//		AngleAreaData[0]=Cencode[1];
//	  AngleAreaData[1]=Bencode[1];
//	  AngleAreaData[2]=Xencode[1];
//		Ready_Send();	
//		
//		STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);Ready_Send();	
//		
//		if(datatemp[0]!=AngleAreaData[0])datatemp[0]=AngleAreaData[0];
//		if(datatemp[1]!=AngleAreaData[1])datatemp[1]=AngleAreaData[1];
//		if(datatemp[2]!=AngleAreaData[2])datatemp[2]=AngleAreaData[2];
//		
//		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);
//	
//	}
//	

///***********************
//【函 数 名】：开机初始复位void Initial_Reset(void)
//【参数说明】: 开机后，读取FLASH中所存的上一次的姿态信息，
//		          牵引床根据该信息对床体进行最初的复位
//***********************/	
//	
//void Initial_Reset(void)
//	{		
//	 STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);
//		
//			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==1)
//				{
//					if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==1)
//						{
//							GPIO_ResetBits(GPIOD,GPIO_Pin_14);
//							delay_ms(50);
//						}
//						else break;
//				}		
//				GPIO_SetBits(GPIOD,GPIO_Pin_14|GPIO_Pin_15);
//				TIM4_Init();					
//		
///*********************************************************************/
//		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)!=0)
//		{
//			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)!=0)
//				{
//					if(datatemp[0]==0){GPIO_ResetBits(GPIOG,GPIO_Pin_6);GPIO_SetBits(GPIOG,GPIO_Pin_7);}
//					else {GPIO_ResetBits(GPIOG,GPIO_Pin_7);GPIO_SetBits(GPIOG,GPIO_Pin_6);}
//					delay_ms(50);
//				}
//				else break;
//			}
//		  GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7);
//			TIM3_Init();
//			
///**********************************************************************/			
//		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)!=0)
//		{
//			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)!=0)
//				{
//					if(datatemp[1]==0){GPIO_ResetBits(GPIOG,GPIO_Pin_4);GPIO_SetBits(GPIOG,GPIO_Pin_5);}
//					else {GPIO_ResetBits(GPIOG,GPIO_Pin_5);GPIO_SetBits(GPIOG,GPIO_Pin_4);}
//					delay_ms(50);
//				}
//				else break;
//			}
//		  GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5);
//			TIM2_Init();
//			
///**********************************************************************/
//		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12)!=0)
//		{
//			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12)!=0)
//				{
//					if(datatemp[2]==0){GPIO_ResetBits(GPIOG,GPIO_Pin_2);GPIO_SetBits(GPIOG,GPIO_Pin_3);}
//					else {GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2);}
//					delay_ms(50);
//				}
//				else break;
//			}
//		  GPIO_SetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3);	
//			TIM1_Init();	
//	}	
		