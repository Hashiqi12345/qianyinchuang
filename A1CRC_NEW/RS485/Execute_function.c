#include "Execute_function.h"
#include "Parse_frame_data.h"
#include "RS485.h"
#include "crc16.h"
#include "led.h"
#include "function.h"
#include "adc.h"
#include "encode.h"
#include "encode1.h"
#include "encode2.h"
#include "encode3.h"
#include "delay.h"
#include "flag_function.h"
/*********************************
按PROTOCOL中的数据协议来执行相应的功能
**************************************/
u8 encode2_Reset = 0;
u8 encode3_Reset = 0;

void Execute_function_configure(void)
{
		if(PROTOCOL[0]==0XAF&&PROTOCOL[1]==0XA1&&PROTOCOL[2]==0X00)//&&RX_CRC==CRCtemp
			{
				u8 function;
				
				function=PROTOCOL[3];
				
				delay_us(4);
				
				switch (function)
					{
						/************************传感器**************************/
						case 0XCA:encode1_Send_Data();break;
						case 0XCB:encode2_Send_Data();break;
						case 0XCC:encode3_Send_Data();break;
						case 0XCD:encode_Send_Data();break;
						case 0XCE:ADC_Send_Data();break;
						
						/***********************成角ANGLE1**********************/
						case 0XBC://STOP
							ANGLE1_function(2);break;
						
						case 0XB0://向上运动
							ANGLE1_function(0);break;//encode1_Send_Data();
						
            case 0XB1://向下运动
							ANGLE1_function(1);break;//encode1_Send_Data();
						
						case 0XB9://成角复位
							ANGLE1_function(4);break;
						
						case 0XB2://按上位机设定参数执行动作
							ENCODE_function(0);break;
						
						/*********************摆角ANGLE2**************************/
						case 0XBD://STOP
							ANGLE2_function(2); break;
						
            case 0XB3://向左运动
							ANGLE2_function(0);break;//encode2_Send_Data();
						
						case 0XB4://向右运动
							ANGLE2_function(1);break;//encode2_Send_Data();
						
						case 0XBA://摆角复位
							ANGLE2_function(4);break;
						
            case 0XB5://按上位机设定参数执行动作
							ENCODE_function(1);break;
						
						/***********************旋角ANGLE3*********************/
						case 0XBE://STOP
							ANGLE3_function(2);break;
						
						case 0XB6://正
							ANGLE3_function(0);break;//encode3_Send_Data();
						
            case 0XB7://反
							ANGLE3_function(1);break;//encode3_Send_Data();
						
						case 0XBB://旋角复位
							ANGLE3_function(4);break;
						
						case 0XB8://按上位机设定参数执行动作
							ENCODE_function(2);break;
						
						/***********************牵引****************************/
					  case 0XC8://牵引复位
							DISTANCE_function(4);encode();break;
						
						case 0XC9://STOP
							DISTANCE_function(2);encode();break;
						
						//慢牵
            case 0XC0://牵引
							mode=0;DISTANCE_function(0);encode_Send_Data();break;//encode_Send_Data();
						
						case 0XC1://回复
							DISTANCE_function(1);encode_Send_Data();break;//encode_Send_Data();
						
            case 0XC2://按上位机设定参数执行动作
							mode=0;ENCODE_function(3);break;
						
						case 0XC3:ADC_NS();ADC_Send_Data();break;
						
						//快牵
            case 0XC4://牵引
							mode=0;
						  GPIO_ResetBits(GPIOD,GPIO_Pin_14);GPIO_SetBits(GPIOD,GPIO_Pin_15);
						  delay_ms(100);
						  GPIO_SetBits(GPIOD,GPIO_Pin_14|GPIO_Pin_15);
						  break;//encode_Send_Data();
						
//						case 0XC5://回复
//							DISTANCE_function(1);encode_Send_Data();break;
						
//            case 0XC6://按上位机设定参数执行动作
//							ENCODE_function(4);break;
						
//						case 0XC7:ADC_NS();ADC_Send_Data();break;
						
						case 0XC5://按摩模式下床体的位移
							mode=1;DISTANCE_function(0);break;//encode_Send_Data();
						case 0XC6://按摩模式下床体的位移，按上位机设定参数执行动作
							mode=1;ENCODE_function(4);break;
						
						/***********************整体复位****************************/
						//整体复位
						case 0XD0:RESET_PLACE();break;
						
						/***********************急停****************************/
						//急停
					  case 0XD1:STOP();break;
						
						case 0XD2:encode2_Reset = 1;GPIO_ResetBits(GPIOG,GPIO_Pin_4);GPIO_SetBits(GPIOG,GPIO_Pin_5);//B4
							//user_Encode2_ResetL();
						break;
						case 0XD3:encode2_Reset = 1;GPIO_ResetBits(GPIOG,GPIO_Pin_5);GPIO_SetBits(GPIOG,GPIO_Pin_4);//B3
							//user_Encode2_ResetR();
						break;
						
						case 0XD4:encode3_Reset = 1;GPIO_ResetBits(GPIOG,GPIO_Pin_2);GPIO_SetBits(GPIOG,GPIO_Pin_3);//B6
						break;
						case 0XD5:encode3_Reset = 1;GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2);//B7
						break;
						
            default:break;
					}
			}
			delay_us(5);
	}


u8 function_flag=0;
u8 base_function_flag=0;
u8 reset_function_flag=0;
u8 date_function_flag=0;
u8 setdata_function_flag=0;		
void flag_function_configure(void)
{
		if(PROTOCOL[0]==0XAF&&PROTOCOL[1]==0XA1&&PROTOCOL[2]==0X00&&RX_CRC==CRCtemp)//
			{
				u8 function;
				
				function=PROTOCOL[3];
				BREAK_FLAG=1;

				delay_us(10);
				
				switch (function)
					{			
					  case 0X11://SYS RESET
						  function_flag=1;base_function_flag=0X11;break;						
						/***********************成角ANGLE1**********************/
						case 0XBC://STOP
							function_flag=1;base_function_flag=0XBC;break;
						case 0XB0://向上运动
							function_flag=1;base_function_flag=0XB0;break;
            case 0XB1://向下运动
							function_flag=1;base_function_flag=0XB1;break;
						case 0XB2://按上位机设定参数执行动作
							function_flag=4;setdata_function_flag=0XB2;break;
						
						/*********************摆角ANGLE2**************************/
						case 0XBD://STOP
							function_flag=1;base_function_flag=0XBD;break;
						
            case 0XB3://向左运动
							function_flag=1;base_function_flag=0XB3;break;
						case 0XB4://向右运动
							function_flag=1;base_function_flag=0XB4;break;
            case 0XB5://按上位机设定参数执行动作
							function_flag=4;setdata_function_flag=0XB5;BREAK_FLAG=0;break;
						
						/***********************旋角ANGLE3*********************/
						case 0XBE://STOP
							function_flag=1;base_function_flag=0XBE;break;
						case 0XB6://正
							function_flag=1;base_function_flag=0XB6;break;
            case 0XB7://反
							function_flag=1;base_function_flag=0XB7;break;
						case 0XB8://按上位机设定参数执行动作
							function_flag=4;setdata_function_flag=0XB8;BREAK_FLAG=0;break;
						
						/***********************牵引****************************/			
						case 0XC9://STOP
							function_flag=1;base_function_flag=0XC9;break;
            case 0XC0://慢牵牵引
							function_flag=1;base_function_flag=0XC0;break;
						case 0XC1://回复
							function_flag=1;base_function_flag=0XC1;break;
						case 0XC2://按上位机设定参数执行动作
							function_flag=4;setdata_function_flag=0XC2;BREAK_FLAG=0;break;
						
						/***********************急停****************************/      
						case 0XD1://急停
							function_flag=1;base_function_flag=0XD1;break;
					 /************************复位****************************/
						case 0XB9://成角复位
							function_flag=2;reset_function_flag=0XB9;break;
						case 0XBA://摆角复位
							function_flag=2;reset_function_flag=0XBA;break;
						case 0XBB://旋角复位
							function_flag=2;reset_function_flag=0XBB;break;
						case 0XC8://牵引复位
							function_flag=2;reset_function_flag=0XC8;break;
						case 0XD2:
							function_flag=2;reset_function_flag=0XD2;break;
						case 0XD3:
							function_flag=2;reset_function_flag=0XD3;break;
						case 0XD4:
							function_flag=2;reset_function_flag=0XD4;break;
						case 0XD5:
							function_flag=2;reset_function_flag=0XD5;break;
						case 0XD6:
							function_flag=2;reset_function_flag=0XD6;break;
						case 0XD7:
							function_flag=2;reset_function_flag=0XD7;break;
					 /************************数据****************************/
						case 0XCA:
							function_flag=3;date_function_flag=0XCA;break;//angulation
						case 0XCB:
							function_flag=3;date_function_flag=0XCB;break;//swingangle
						case 0XCC:
							function_flag=3;date_function_flag=0XCC;break;//rotationangle
						case 0XCD:
							function_flag=3;date_function_flag=0XCD;break;//tow
						case 0XCE:
							function_flag=3;date_function_flag=0XCE;break;
						
            default:break;
					}		
			}
			delay_us(5);
	}
				
	
















