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
��PROTOCOL�е�����Э����ִ����Ӧ�Ĺ���
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
						/************************������**************************/
						case 0XCA:encode1_Send_Data();break;
						case 0XCB:encode2_Send_Data();break;
						case 0XCC:encode3_Send_Data();break;
						case 0XCD:encode_Send_Data();break;
						case 0XCE:ADC_Send_Data();break;
						
						/***********************�ɽ�ANGLE1**********************/
						case 0XBC://STOP
							ANGLE1_function(2);break;
						
						case 0XB0://�����˶�
							ANGLE1_function(0);break;//encode1_Send_Data();
						
            case 0XB1://�����˶�
							ANGLE1_function(1);break;//encode1_Send_Data();
						
						case 0XB9://�ɽǸ�λ
							ANGLE1_function(4);break;
						
						case 0XB2://����λ���趨����ִ�ж���
							ENCODE_function(0);break;
						
						/*********************�ڽ�ANGLE2**************************/
						case 0XBD://STOP
							ANGLE2_function(2); break;
						
            case 0XB3://�����˶�
							ANGLE2_function(0);break;//encode2_Send_Data();
						
						case 0XB4://�����˶�
							ANGLE2_function(1);break;//encode2_Send_Data();
						
						case 0XBA://�ڽǸ�λ
							ANGLE2_function(4);break;
						
            case 0XB5://����λ���趨����ִ�ж���
							ENCODE_function(1);break;
						
						/***********************����ANGLE3*********************/
						case 0XBE://STOP
							ANGLE3_function(2);break;
						
						case 0XB6://��
							ANGLE3_function(0);break;//encode3_Send_Data();
						
            case 0XB7://��
							ANGLE3_function(1);break;//encode3_Send_Data();
						
						case 0XBB://���Ǹ�λ
							ANGLE3_function(4);break;
						
						case 0XB8://����λ���趨����ִ�ж���
							ENCODE_function(2);break;
						
						/***********************ǣ��****************************/
					  case 0XC8://ǣ����λ
							DISTANCE_function(4);encode();break;
						
						case 0XC9://STOP
							DISTANCE_function(2);encode();break;
						
						//��ǣ
            case 0XC0://ǣ��
							mode=0;DISTANCE_function(0);encode_Send_Data();break;//encode_Send_Data();
						
						case 0XC1://�ظ�
							DISTANCE_function(1);encode_Send_Data();break;//encode_Send_Data();
						
            case 0XC2://����λ���趨����ִ�ж���
							mode=0;ENCODE_function(3);break;
						
						case 0XC3:ADC_NS();ADC_Send_Data();break;
						
						//��ǣ
            case 0XC4://ǣ��
							mode=0;
						  GPIO_ResetBits(GPIOD,GPIO_Pin_14);GPIO_SetBits(GPIOD,GPIO_Pin_15);
						  delay_ms(100);
						  GPIO_SetBits(GPIOD,GPIO_Pin_14|GPIO_Pin_15);
						  break;//encode_Send_Data();
						
//						case 0XC5://�ظ�
//							DISTANCE_function(1);encode_Send_Data();break;
						
//            case 0XC6://����λ���趨����ִ�ж���
//							ENCODE_function(4);break;
						
//						case 0XC7:ADC_NS();ADC_Send_Data();break;
						
						case 0XC5://��Ħģʽ�´����λ��
							mode=1;DISTANCE_function(0);break;//encode_Send_Data();
						case 0XC6://��Ħģʽ�´����λ�ƣ�����λ���趨����ִ�ж���
							mode=1;ENCODE_function(4);break;
						
						/***********************���帴λ****************************/
						//���帴λ
						case 0XD0:RESET_PLACE();break;
						
						/***********************��ͣ****************************/
						//��ͣ
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
						/***********************�ɽ�ANGLE1**********************/
						case 0XBC://STOP
							function_flag=1;base_function_flag=0XBC;break;
						case 0XB0://�����˶�
							function_flag=1;base_function_flag=0XB0;break;
            case 0XB1://�����˶�
							function_flag=1;base_function_flag=0XB1;break;
						case 0XB2://����λ���趨����ִ�ж���
							function_flag=4;setdata_function_flag=0XB2;break;
						
						/*********************�ڽ�ANGLE2**************************/
						case 0XBD://STOP
							function_flag=1;base_function_flag=0XBD;break;
						
            case 0XB3://�����˶�
							function_flag=1;base_function_flag=0XB3;break;
						case 0XB4://�����˶�
							function_flag=1;base_function_flag=0XB4;break;
            case 0XB5://����λ���趨����ִ�ж���
							function_flag=4;setdata_function_flag=0XB5;BREAK_FLAG=0;break;
						
						/***********************����ANGLE3*********************/
						case 0XBE://STOP
							function_flag=1;base_function_flag=0XBE;break;
						case 0XB6://��
							function_flag=1;base_function_flag=0XB6;break;
            case 0XB7://��
							function_flag=1;base_function_flag=0XB7;break;
						case 0XB8://����λ���趨����ִ�ж���
							function_flag=4;setdata_function_flag=0XB8;BREAK_FLAG=0;break;
						
						/***********************ǣ��****************************/			
						case 0XC9://STOP
							function_flag=1;base_function_flag=0XC9;break;
            case 0XC0://��ǣǣ��
							function_flag=1;base_function_flag=0XC0;break;
						case 0XC1://�ظ�
							function_flag=1;base_function_flag=0XC1;break;
						case 0XC2://����λ���趨����ִ�ж���
							function_flag=4;setdata_function_flag=0XC2;BREAK_FLAG=0;break;
						
						/***********************��ͣ****************************/      
						case 0XD1://��ͣ
							function_flag=1;base_function_flag=0XD1;break;
					 /************************��λ****************************/
						case 0XB9://�ɽǸ�λ
							function_flag=2;reset_function_flag=0XB9;break;
						case 0XBA://�ڽǸ�λ
							function_flag=2;reset_function_flag=0XBA;break;
						case 0XBB://���Ǹ�λ
							function_flag=2;reset_function_flag=0XBB;break;
						case 0XC8://ǣ����λ
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
					 /************************����****************************/
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
				
	
















