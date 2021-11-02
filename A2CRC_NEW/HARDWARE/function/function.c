#include "sys.h"
#include "function.h"
#include "ON_OFF.h"
#include "2803.h"  
#include "Debug_PIN.h"
#include "adc.h"
#include "delay.h"
#include "Execute_function.h"
#include "RS485.h"
#include "Parse_frame_data.h"
#include "led.h"
#include "PWM.h"
#include "TIM.h"
#include "flag_function.h"

//左侧推顶

void Push_L(u8 numL)
	{
	switch(numL)
		{
			case 0:OUT5=1;OUT6=0; break;//向上
			case 1:OUT6=1;OUT5=0; break;//向下
			case 2:
				OUT5=0;OUT6=0;
			
				pack_protocol(10);
				RS485_Send_Data(RS485_TX_BUF,8);
			  delay_us(5);
			
				break;//停止
			case 3:break;
			
		 	case 4://向下停 
			 OUT6=1;
		   delay_ms(100);
		   OUT6=0;break;
		 
			case 5://向上停 
			 OUT5=1;
		   delay_ms(100);
		   OUT5=0;break;
		 		 
		  case 6://向下 触发磁性接近开关停
			 OUT6=0;
			 while(ON_OFF4==0)
				 {
					 if(BREAK_FLAG!=0)break;
					 
					 if(ON_OFF4==0)
						 OUT5=1;
           else 
						 break;delay_ms(10);
					}
				 OUT6=0;	 
	       break;
						 
		  case 7://向上 触发磁性接近开关停
       OUT6=0;			 
			 while(ON_OFF4==0)
				 {
					 if(BREAK_FLAG!=0)break;
					 
					 if(ON_OFF4==0)
						 OUT6=1;
           else 
						 break;delay_ms(10);
				 }
				 OUT6=0;
				 break;

		  default:break;
		}
	}	
	
//右侧推顶
void Push_R(u8 numR)
	{
	switch(numR)
		{
			case 0:OUT7=1;OUT8=0; break;//向上
			case 1:OUT8=1;OUT7=0; break;//向下
			case 2:
				OUT8=0;OUT7=0;
				
				pack_protocol(11);
				RS485_Send_Data(RS485_TX_BUF,8);
				pack_protocol(11);
				RS485_Send_Data(RS485_TX_BUF,8);
			  delay_us(5);
			
				break;//停止
			case 3:break;
			
		 	case 4:
			 OUT7=1;
		   delay_ms(100);
		   OUT7=0;break;//向下停 
		 
			case 5:
			 OUT8=1;
		   delay_ms(100);
		   OUT8=0;break;//向下停 
		 		 
		  case 6://向下 触发磁性接近开关停
			 OUT8=0;
			 while(ON_OFF3==0)
				 {
					 if(BREAK_FLAG!=0)break;
					 
					 if(ON_OFF3==0)
						 OUT7=1;
           else 
						 break;delay_ms(10);
					}
				 OUT7=0;	 
	       break;
						 
		  case 7://向上 触发磁性接近开关停
       OUT7=0;			 
			 while(ON_OFF3==0)
				 {
					 if(BREAK_FLAG!=0)break;
					 
					 if(ON_OFF3==0)
						 OUT8=1;
           else 
						 break;delay_ms(10);
				 }
				 OUT8=0;
				 break;

		  default:break;
		}
	}
	
/**************************NEXT PART*********************************/
	
//左侧小电机 振幅
void Amplitude_L(u8 numL)
	{ 
		switch (numL)
    {
		 case 0:
			 out1_2=0;out1_1=0;
			 delay_ms(500);
		   out1_2=1;out1_1=0;break;//向上
		 case 1:
			 out1_2=0;out1_1=0;
			 delay_ms(500);
			 out1_1=1;out1_2=0;break;//向下
		 case 2:out1_2=0;out1_1=0;
					  pack_protocol(6);
					  RS485_Send_Data(RS485_TX_BUF,8);
			      delay_us(5);
		 				pack_protocol(6);
					  RS485_Send_Data(RS485_TX_BUF,8);
			break;//停止
		
		 case 3://向下 触发接近开关停
			 if(ON_OFF1==0)
				 {
					 out1_1=1;out1_2=0;
				 }
				 else
				 {
					 out1_1=0;out1_2=0;
					 
					 pack_protocol(2);
					 RS485_Send_Data(RS485_TX_BUF,8);
					 delay_us(5);
					 pack_protocol(2);
					 RS485_Send_Data(RS485_TX_BUF,8);
					 
					 clear_resetfunction_flag();
				 }					 
				 break;

		 default:break;
    }
	}
	
//右侧小电机 振幅
void Amplitude_R(u8 numR)
	{ 
		switch (numR)
    {
		 case 0:
			 out1_4=0;out1_3=0;
		   delay_ms(500);
			 out1_4=1;out1_3=0;break;//向上
		 case 1:
			 out1_4=0;out1_3=0;
		   delay_ms(500);
			 out1_3=1;out1_4=0;break;//向下
		 case 2:out1_4=0;out1_3=0;
			      pack_protocol(7);
					  RS485_Send_Data(RS485_TX_BUF,8);
			      delay_us(5);    
			      pack_protocol(7);
					  RS485_Send_Data(RS485_TX_BUF,8);		 
			break;//停止
			
		 case 3://向下 触发接近开关停					 
			 if(ON_OFF2==0)
				 {
					 out1_3=1;out1_4=0;
				 }
				 else
				 {
					 out1_3=0;out1_4=0;
					 
					 pack_protocol(3);
					 RS485_Send_Data(RS485_TX_BUF,8);
					 delay_us(5);
					 pack_protocol(3);
					 RS485_Send_Data(RS485_TX_BUF,8);
					 
					 clear_resetfunction_flag();					 
				 }
				 break;
		 
		 default:break;
    }
	}

/**************************NEXT PART*********************************/
	u16 pwmL_ch1=1000;
	u16 pwmR_ch4=1000;	
	
//大电机电源
void MOTOR_POWER(u8 num)
	{
		switch (num)
    {
    	case 0://开启电源
				OUT4=1;
			break;
			
    	case 1://关闭电源
    		OUT4=0;
			break;
			
    	default:break;
    }
	}	
	
//左侧大电机 MOTOR
	u8 change_flagL=0;
	u8 change_flagR=0;
	
void MOTOR_L(u8 numL)
	{ 
		switch (numL)
    {
		 case 0://启动
			 IN1=1;	
		 break;
		 
		 case 1://停止
			 IN1=0;
		   TIM_Cmd(TIM3,DISABLE);
			 TIM3->CNT = 0;
		 
		   pack_protocol(8);
			 RS485_Send_Data(RS485_TX_BUF,8);
			 delay_us(5);
		 break;	
		 
		 case 2://复位
			 change_flagL=1;
		   pwmL_ch1=1000;
		   TIM_SetCompare1(TIM1,pwmL_ch1);
		   if(ON_OFF6 != 1)
				 {
					 IN1=1;
				 }
				 else
					 {
						IN1=0;
						 
						pack_protocol(4);
						RS485_Send_Data(RS485_TX_BUF,8);
						delay_us(5);
						pack_protocol(4);
						RS485_Send_Data(RS485_TX_BUF,8);
						 
						clear_resetfunction_flag();
					 }

		 break;				
		 
		 default:break;
    }
	}

//右侧大电机 MOTOR	
void MOTOR_R(u8 numR)
	{ 
		switch (numR)
    {
		 case 0://启动
			 IN3=1;
		 break;
		 
		 case 1://停止
			 IN3=0;
		   TIM_Cmd(TIM4,DISABLE);
			 TIM4->CNT = 0;
		   pack_protocol(9);
			 RS485_Send_Data(RS485_TX_BUF,8);
			 delay_us(5);
		 break;	
		 
		 case 2://复位
			 change_flagR=1;
		   pwmR_ch4=1000;
		   TIM_SetCompare4(TIM1,pwmR_ch4);   

			if(ON_OFF5 != 1)
					{
						IN3=1;
					}
				 else
				 {
					IN3=0;
					 
					pack_protocol(5);
			    RS485_Send_Data(RS485_TX_BUF,8);	
					delay_us(5);
					pack_protocol(5);
			    RS485_Send_Data(RS485_TX_BUF,8);	
          clear_resetfunction_flag();				 	 
				 }
					

			 break;					
		 
		 default:break;
    }
	}
	
/**************************NEXT PART*********************************/	

void PWM_L(u8 numL)
	{
		switch (numL)
    {
    	case 0:
				TIM_SetCompare1(TIM1,pwmL_ch1);
    		break;
			
    	case 1:
				if(pwmL_ch1<6500)
					{
						pwmL_ch1+=500;TIM_SetCompare1(TIM1,pwmL_ch1);
					}
					else 
						{
							pwmL_ch1=6500;TIM_SetCompare1(TIM1,pwmL_ch1);
						}
    		break;
						
			case 2:
				if(pwmL_ch1>1000)
					{
						pwmL_ch1-=500;TIM_SetCompare1(TIM1,pwmL_ch1);
					}
					else 
						{
							pwmL_ch1=1000;TIM_SetCompare1(TIM1,pwmL_ch1);
						}
    		break;
						
			case 3:
				pwmL_ch1=PWM_VALUE;TIM_SetCompare1(TIM1,pwmL_ch1);
			break;
			
    	default:break;
    }
	}
	

void PWM_R(u8 numR)
	{
		switch (numR)
    {
    	case 0:
				TIM_SetCompare4(TIM1,pwmR_ch4);
    		break;
			
    	case 1:
				if(pwmR_ch4<=6500)
					{
						pwmR_ch4+=500;TIM_SetCompare4(TIM1,pwmR_ch4);
					}
					else 
						{
							pwmR_ch4=6500;TIM_SetCompare4(TIM1,pwmR_ch4);
						}
    		break;
						
			case 2:
				if(pwmR_ch4>1000)
					{
						pwmR_ch4-=500;TIM_SetCompare4(TIM1,pwmR_ch4);
					}
					else
						{
							pwmR_ch4=1000;TIM_SetCompare4(TIM1,pwmR_ch4);
						}
				break;
						
		  case 3:
       pwmR_ch4=PWM_VALUE;TIM_SetCompare4(TIM1,pwmR_ch4);				
    		break;
			
    	default:break;
    }
	}

/**************************NEXT PART*********************************/	

//急停
void STOP(void)
	{
		MOTOR_L(1);MOTOR_R(1);
		Amplitude_L(2);Amplitude_R(2);
		Push_L(2);Push_R(2);
	}	
	
/**************************NEXT PART*********************************/	

//复位
void RESET_function(void)
	{
		static u8 a = 0;
		
		change_flagL=1;
		pwmL_ch1=1000;
		TIM_SetCompare1(TIM1,pwmL_ch1);
		
		change_flagR=1;
		pwmR_ch4=1000;
		TIM_SetCompare4(TIM1,pwmR_ch4); 
		
		
		switch (a)
		{
			case 0:
				if(ON_OFF6 != 1){ IN1=1;}
				else{IN1=0;a =1;}
				break;
				
			case 1:
				if(ON_OFF5 != 1){ IN3=1;}
				else{IN3=0;a =2;}
				break;

			case 2:
			  if(ON_OFF1 ==0){out1_1=1;out1_2=0;}
				else{
				out1_1=0;out1_2=0;a=3;
//				pack_protocol(13);
//				RS485_Send_Data(RS485_TX_BUF,8);
//				pack_protocol(13);
//				RS485_Send_Data(RS485_TX_BUF,8);
//				delay_us(5);
//				clear_resetfunction_flag();
				}	
				break;
				
			case 3:
			  if(ON_OFF2 ==0){out1_3=1;out1_4=0;}
				else{
				out1_3=0;out1_4=0;a=0;
				pack_protocol(13);
				RS485_Send_Data(RS485_TX_BUF,8);
				delay_us(5);
				pack_protocol(13);
				RS485_Send_Data(RS485_TX_BUF,8);
				clear_resetfunction_flag();
				}
				break;	
				
			default:
				break;
		}
		delay_us(5);
	}			
//		if(ON_OFF6 != 1){IN1 = 1;}//MOTOR_L RUN
//		else
//			{
//				IN1=0;//MOTOR_L STOP

//				if(ON_OFF5 != 1){IN3 = 1;}//MOTOR_R RUN
//				else
//					{
//						IN3=0;//MOTOR_R STOPelse

//						if(ON_OFF1!=1){out1_1=1;out1_2=0;}//Amplitude_L RUN
//						else
//							{
//								out1_1=0;out1_2=0;//Amplitude_L STOP

//								if(ON_OFF2!=1){out1_3=1;out1_4=0;}
//								else
//									{
//										out1_3=0;out1_4=0;//Amplitude_R STOP	
//										pack_protocol(13);
//										RS485_Send_Data(RS485_TX_BUF,8);
//										delay_us(5);
//										clear_resetfunction_flag();
//									}	
//							}									
//					}
//     }
						
/***********************************************************************/
	////L DC
//void DC_L(u8 numL)
//	{
//		switch (numL)
//    {
//		 case 0:GPIO_ResetBits(GPIOD, GPIO_Pin_13);delay_ms(100);break;//+
//		 case 1:GPIO_SetBits(GPIOG, GPIO_Pin_13);break;//停止
//		 case 2:GPIO_ResetBits(GPIOG, GPIO_Pin_5);delay_ms(100);break;//-
//		 case 3:GPIO_SetBits(GPIOG, GPIO_Pin_5);break;//停止 	

//     case 4://设定固定值DC_VALUE，控制DC模块调压
//			 ADC_DCL();
//		 
//		   while(temp_DCL!=DC_VALUE)
//				 {
//					 if(BREAK_FLAG!=0)break;
//					 
//					 if(BREAK_FLAG!=0)break;//外部给赋值1，可强行跳出中断
//					 ADC_DCL();
//					 //pack_protocol(2);RS485_Send_Data(RS485_TX_BUF,6);调试显示

//					 if(temp_DCL>DC_VALUE)
//             {
//						   GPIO_ResetBits(GPIOG, GPIO_Pin_5);
//						   delay_ms(50);
//						   GPIO_SetBits(GPIOG, GPIO_Pin_5);
//						 }
//					else if(temp_DCL<DC_VALUE)
//             {
//						   GPIO_ResetBits(GPIOG, GPIO_Pin_13);
//						   delay_ms(50);
//						   GPIO_SetBits(GPIOG, GPIO_Pin_13);
//						 }	
//					else if(temp_DCL==DC_VALUE)	 break;
//				 }
//				 break;
//			
//		 default:break;
//    }	
//	}
//	
////R DC
//void DC_R(u8 numR)
//	{
//		switch(numR)
//    {
//		 case 0:GPIO_ResetBits(GPIOG, GPIO_Pin_4);delay_ms(100);break;//+
//		 case 1:GPIO_SetBits(GPIOG, GPIO_Pin_4);delay_ms(100);break;//停止 
//		 case 2:GPIO_ResetBits(GPIOG, GPIO_Pin_5);delay_ms(100);break;//-
//		 case 3:GPIO_SetBits(GPIOG, GPIO_Pin_5);delay_ms(100);break;//停止
//			
//     case 4://设定固定值DC_VALUE，控制DC模块调压
//			 ADC_DCR();
//		 
//		   while(temp_DCR!=DC_VALUE)
//				 {
//					 if(BREAK_FLAG!=0)break;//外部给赋值1，可强行跳出中断
//					 ADC_DCR();
//					 //pack_protocol(2);RS485_Send_Data(RS485_TX_BUF,6);调试显示

//					 if(temp_DCR>DC_VALUE)
//             {
//						   GPIO_ResetBits(GPIOG, GPIO_Pin_10);
//						   delay_ms(50);
//						   GPIO_SetBits(GPIOG, GPIO_Pin_10);
//						 }
//					else if(temp_DCR<DC_VALUE)
//             {  
//						   GPIO_ResetBits(GPIOG, GPIO_Pin_9);
//						   delay_ms(50);
//						   GPIO_SetBits(GPIOG, GPIO_Pin_9);
//						 }	
//					else if(temp_DCR==DC_VALUE)	 break;
//				 }
//				 break;
// 			
//		 default:break;
//    }	
//	}	
