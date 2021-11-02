#include "Execute_function.h"
#include "Parse_frame_data.h"
#include "RS485.h"
#include "crc16.h"
#include "led.h"
#include "function.h"
#include "adc.h"
#include "delay.h"
#include "PWM.h"

/*********************************
按PROTOCOL中的数据协议来执行相应的功能
**************************************/
u16 PWM_VALUE;
void Execute_function_configure(void)
{
		if(PROTOCOL[0]==0XAF&&PROTOCOL[1]==0XA2&&PROTOCOL[2]==0X00)//&&RX_CRC==CRCtemp
			{
				u8 function;

				PWM_VALUE=(PROTOCOL[4]<<8)+PROTOCOL[5];
				function=PROTOCOL[3];
				
				//delay_us(4);
				
				switch (function)
					{
						case 0X00:__set_FAULTMASK(1); NVIC_SystemReset();break;
						/*****************************推顶*****************************/
						case 0XE0:Push_L(0);break;//向上 
						case 0XE1:Push_L(1);break;//向下 
						case 0XD4:Push_L(2);break;//停止
						
						case 0XE7:Push_R(0);break;//向上 
						case 0XE8:Push_R(1);break;//向下 
						case 0XD9:Push_R(2);break;//停止
						
						/*****************************振幅*****************************/
						case 0XE2:Amplitude_L(0);break;//向上
						case 0XE3:Amplitude_L(1);break;//向下 
						case 0XE4:Amplitude_L(3);break;//振幅复位
						case 0XD5:Amplitude_L(2);break;//停止
						
						case 0XE9:Amplitude_R(0);break;//向上
						case 0XEA:Amplitude_R(1);break;//向下 
						case 0XEB:Amplitude_R(3);break;//振幅复位
						case 0XDA:Amplitude_R(2);break;//停止
						
	          /*****************************频率*****************************/
						//MOTOR_POWER
					  case 0XDE:MOTOR_POWER(0);RESET_function();break;//开启电源 
						case 0XDF:MOTOR_POWER(1);break;//关闭电源 
								
					  case 0XE5:PWM_L(0);MOTOR_L(0);break;//左侧大电机启动
						case 0XD8:MOTOR_L(1);break;//停止
						case 0XCA:MOTOR_L(2);break;//复位
						
						case 0XEC:PWM_R(0);MOTOR_R(0);break;//右侧大电机启动	
						case 0XDD:MOTOR_R(1);break;//停止
						case 0XCB:MOTOR_R(2);break;//复位
						
						/*****************************调速*****************************/
						case 0XD6:PWM_L(1);break;//左 速 +
						case 0XD7:PWM_L(2);break;//左 速 -
						case 0XEE:PWM_L(3);break;//左 指定速度
							
						case 0XDB:PWM_R(1);break;//右 速 +
						case 0XDC:PWM_R(2);break;//右 速 -
            case 0XEF:PWM_R(3);break;//左 指定速度
						
						//大电机同步启动
//					  case 0XCC:IN1=1;IN3=1;//GPIO_SetBits(GPIOA,GPIO_Pin_7);GPIO_SetBits(GPIOC,GPIO_Pin_5);
//						break;						
						//大电机同步停止
						case 0XCD:
							IN1=0;IN3=0;
						
							pack_protocol(12);
			        RS485_Send_Data(RS485_TX_BUF,8);
			        delay_us(5);
						
						//GPIO_ResetBits(GPIOA,GPIO_Pin_7);GPIO_ResetBits(GPIOC,GPIO_Pin_5);
						break;
						
						//总复位
						case 0XD2:RESET_function();break;
							
						//急停
					  case 0XD3:STOP();break;
						
            default:break;
					}
			}
			delay_us(4);
	}

u8 function_flag=0;
u8 base_function_flag=0;
u8 reset_function_flag=0;
u8 date_function_flag=0;	
void flag_function_configure(void)
{
	if(PROTOCOL[0]==0XAF&&PROTOCOL[1]==0XA2&&PROTOCOL[2]==0X00&&RX_CRC==CRCtemp)//
		{
			u8 function;
			
			function=PROTOCOL[3];

			delay_us(10);
			
			switch (function)
				{	
					case 0X11://SYS RESET
						function_flag=1;base_function_flag=0X11;break;
					
/************************************base_function**********************************/					
					/*****************************推顶*****************************/
					case 0XE0://向上
						function_flag=1;base_function_flag=0XE0;break;	
					case 0XE1://向下 
						function_flag=1;base_function_flag=0XE1;break;	
					case 0XD4://停止
						function_flag=1;base_function_flag=0XD4;break;	
					
					case 0XE7://向上 
						function_flag=1;base_function_flag=0XE7;break;	
					case 0XE8://向下 
						function_flag=1;base_function_flag=0XE8;break;	
					case 0XD9://停止
						function_flag=1;base_function_flag=0XD9;break;	
					
					/*****************************振幅*****************************/
					case 0XE2://向上
						function_flag=1;base_function_flag=0XE2;break;
					case 0XE3://向下 
						function_flag=1;base_function_flag=0XE3;break;
					case 0XD5://停止
						function_flag=1;base_function_flag=0XD5;break;
					
					case 0XE9://向上
						function_flag=1;base_function_flag=0XE9;break;
					case 0XEA://向下 
						function_flag=1;base_function_flag=0XEA;break;
					case 0XDA://停止
						function_flag=1;base_function_flag=0XDA;break;
					
					 /*****************************频率*****************************/
					case 0XDF://关闭电源
						function_flag=1;base_function_flag=0XDF;break;
							
					case 0XE5://左侧大电机启动
						function_flag=1;base_function_flag=0XE5;break;
					case 0XD8://停止
						function_flag=1;base_function_flag=0XD8;break;
					
					case 0XEC://右侧大电机启动	
						function_flag=1;base_function_flag=0XEC;break;
					case 0XDD://停止
						function_flag=1;base_function_flag=0XDD;break;
					
					 /*****************************调速*****************************/
					case 0XD6://左 速 +
						function_flag=1;base_function_flag=0XD6;break;
					case 0XD7://左 速 -
						function_flag=1;base_function_flag=0XD7;break;
					case 0XEE://左 指定速度
						PWM_VALUE=(PROTOCOL[4]<<8)+PROTOCOL[5];function_flag=1;base_function_flag=0XEE;break;
						
					case 0XDB://右 速 +
						function_flag=1;base_function_flag=0XDB;break;
					case 0XDC://右 速 -
						function_flag=1;base_function_flag=0XDC;break;
					case 0XEF://左 指定速度		
						PWM_VALUE=(PROTOCOL[4]<<8)+PROTOCOL[5];function_flag=1;base_function_flag=0XEF;break;
					
					//大电机同步启动
		      case 0XCC:
						function_flag=1;base_function_flag=0XCC;break;//
					//大电机同步停止
					case 0XCD:
						function_flag=1;base_function_flag=0XCD;break;
								
					//急停
					case 0XD3:
						function_flag=1;base_function_flag=0XD3;break;
					
/************************************reset_function**********************************/	
					case 0XDE://开启大电机电源
						function_flag=2;reset_function_flag=0XDE;break;
					
					case 0XE4://振幅复位
						function_flag=2;reset_function_flag=0XE4;break;
					case 0XEB:
						function_flag=2;reset_function_flag=0XEB;break;
					
					
					case 0XCA://大电机复位
						function_flag=2;reset_function_flag=0XCA;break;
					case 0XCB:
						function_flag=2;reset_function_flag=0XCB;break;
					
					
					case 0XD2://总复位
						function_flag=2;reset_function_flag=0XD2;break;
					default:break;
				}		
		}
		delay_us(5);	
	
	
}	

				
	
















