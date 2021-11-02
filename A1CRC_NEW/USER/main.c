#include "stm32f10x_it.h" 
#include "delay.h"
#include "sys.h"
#include "usart.h"

/********
HARDWARE
**********/
#include "port.h"
#include "adc.h"
#include "LED.h"
#include "function.h"
#include "flag_function.h"
//#include "stmflash.h"
//#include "wdg.h"

/********
ENCODE
**********/
#include "encode.h"
#include "encode1.h"
#include "encode2.h"
#include "encode3.h"

/********
RS485
**********/
#include "RS485.h"
#include "crc16.h"	
#include "Parse_frame_data.h"
#include "Execute_function.h"

 int main(void)
 {
	delay_init();	    	 //��ʱ������ʼ��
	 
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
  RS485_init(115200); 
	 
	PORT_GPIO_Config();
	ON_OFF_Init();
  LED_Init();	 	
	TIM4_Init();  //��դ��ʱ���������ӿڳ�ʼ�� 	
	TIM3_Init();	//������1��Ӧ�Ķ�ʱ��
	TIM2_Init();	//������2��Ӧ�Ķ�ʱ�� 
	TIM1_Init();	//������3��Ӧ�Ķ�ʱ��
	Adc_Init();		  		//ADC��ʼ��    
	//IWDG_Init(5,1250);
	Ready_Send();	 

	 while(1)
	{
		LED0=~LED0;
		delay_ms(50);	
		
		//update_flag();
		
		switch (function_flag)
    {
    	case 1:base_function();delay_us(5);
    		break;
    	case 2:reset_function();delay_us(5);
    		break;
			case 3:date_function();delay_us(5);
    		break;
			case 4:setdata_function();delay_us(5);
    		break;
    	default:
    		break;
    }
				 
		encode1_limit();delay_us(5);
		encode2_limit();delay_us(5);
		encode3_limit();delay_us(5);
		encode_limit(mode);delay_us(5);		 	 
	} 
}


