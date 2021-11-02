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

void clear_basefunction_flag(void)
{
	function_flag=0;base_function_flag=0;
}

void clear_resetfunction_flag(void)
{
	function_flag=0;reset_function_flag=0;
}

void clear_datefunction_flag(void)
{
	function_flag=0;date_function_flag=0;
}

void clear_setdatafunction_flag(void)
{
	function_flag=0;setdata_function_flag=0;
}

void update_flag(void)
{
	updateflag = function_flag;
  update_base_flag = base_function_flag;
  update_reset_flag = reset_function_flag;
  update_date_flag = date_function_flag;
}

void base_function(void)
{
	switch (base_function_flag)
  {
		case 0X11:__set_FAULTMASK(1); NVIC_SystemReset();break;
		/***********************成角ANGLE1**********************/
		case 0XBC://STOP
			ANGLE1_function(2);
			break;	
		case 0XB0://向上运动
			ANGLE1_function(0);
			break;//encode1_Send_Data();
		case 0XB1://向下运动
			ANGLE1_function(1);
			break;//encode1_Send_Data();
		
		/*********************摆角ANGLE2**************************/
		case 0XBD://STOP
			ANGLE2_function(2);
			break;
		case 0XB3://向左运动
			ANGLE2_function(0);
			break;//encode2_Send_Data();
		case 0XB4://向右运动
			ANGLE2_function(1);
			break;//encode2_Send_Data();
		
		/***********************旋角ANGLE3*********************/
		case 0XBE://STOP
			ANGLE3_function(2);
			break;
		case 0XB6://正
			ANGLE3_function(0);
			break;//encode3_Send_Data();
		case 0XB7://反
			ANGLE3_function(1);
			break;//encode3_Send_Data();
		
		/***********************牵引****************************/
		case 0XC9://STOP
			DISTANCE_function(2);
			encode();
			break;
		case 0XC0://慢牵牵引
			mode=0;
			DISTANCE_function(0);
			break;//encode_Send_Data();
		case 0XC1://回复
		  DISTANCE_function(1);encode_Send_Data();break;//encode_
		
		/***********************急停****************************/      
    case 0XD1://急停
			STOP();
			break;
		default:
  		break;
  }
	clear_basefunction_flag();
}

void reset_function(void)
{
	switch (reset_function_flag)
  {
		case 0XB9://成角复位
			ANGLE1_function(4);break;
		case 0XBA://摆角复位
			ANGLE2_function(4);break;
		case 0XBB://旋角复位
			ANGLE3_function(4);break;
		case 0XC8://牵引复位
			DISTANCE_function(4);break;
		
		case 0XD2:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);GPIO_SetBits(GPIOG,GPIO_Pin_5);//B4
		  if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) != 0)
				{
					GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5);          
					TIM2_Init();
					pack_protocol(15);
		      RS485_Send_Data(RS485_TX_BUF,8);
					clear_resetfunction_flag();
				}	
		  break;
		case 0XD3:
			GPIO_ResetBits(GPIOG,GPIO_Pin_5);GPIO_SetBits(GPIOG,GPIO_Pin_4);//B3
		  if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) != 0)
				{
					GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5);          
					TIM2_Init();
					pack_protocol(15);
		      RS485_Send_Data(RS485_TX_BUF,8);
					clear_resetfunction_flag();
				}	
		  break;	
		
		case 0XD4:
			GPIO_ResetBits(GPIOG,GPIO_Pin_2);GPIO_SetBits(GPIOG,GPIO_Pin_3);//B6
			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10) != 0)
				{
					GPIO_SetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3);
					TIM1_Init();
					pack_protocol(16);
					RS485_Send_Data(RS485_TX_BUF,8);
					clear_resetfunction_flag();
				}	 
		  break;
		case 0XD5:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2);//B7
			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10) != 0)
				{
					GPIO_SetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3);
					TIM1_Init();
					pack_protocol(16);
					RS485_Send_Data(RS485_TX_BUF,8);
					clear_resetfunction_flag();
				}

		case 0XD6://UP
			encode1();
			if(Cencode[2] == 0)
				{
					GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7);
					TIM3_Init();			
					delay_us(5);	
					pack_protocol(17);
					RS485_Send_Data(RS485_TX_BUF,8);
					clear_resetfunction_flag();
				}
      else{GPIO_ResetBits(GPIOG,GPIO_Pin_7);GPIO_SetBits(GPIOG,GPIO_Pin_6);}				
		  break;
		case 0XD7://DOWN
			encode1();
			if(Cencode[2] == 0)
				{
					GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7);
					TIM1_Init();
					pack_protocol(17);
					RS485_Send_Data(RS485_TX_BUF,8);
					clear_resetfunction_flag();
				}
      else{GPIO_ResetBits(GPIOG,GPIO_Pin_6);GPIO_SetBits(GPIOG,GPIO_Pin_7);} 				
		  break;				
  	default:
  		break;
  }
}

void date_function(void)
{
	switch (date_function_flag)
  {
		case 0XCA:encode1_Send_Data();clear_basefunction_flag();break;//angulation
		case 0XCB:encode2_Send_Data();clear_basefunction_flag();break;//swingangle
		case 0XCC:encode3_Send_Data();clear_basefunction_flag();break;//rotationangle
		case 0XCD:encode_Send_Data();clear_basefunction_flag();break;//tow
		case 0XCE:ADC_Send_Data();clear_basefunction_flag();break;		
  	default:
  		break;
  }

}

void setdata_function(void)
{
	switch (setdata_function_flag)
  {
		case 0XB2:
			ENCODE_function(0);break;//Cangle
		case 0XB5:
		//	ENCODE_function(1);break;//Bangle
		  Baijiao_function();break;
		case 0XB8:
			//ENCODE_function(2);break;//Xangle
		  Xuanjiao_function();break;
		case 0XC2:
			QianYin_function();break;//	QY
  	default:
  		break;
  }
//clear_setdatafunction_flag();
}