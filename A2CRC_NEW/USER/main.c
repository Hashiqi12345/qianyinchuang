#include "stm32f10x.h"
#include "wdg.h"
#include "sys.h"
#include "delay.h"
#include "exti.h"
#include "Debug_PIN.h"
#include "TIM.h"
#include "led.h"
#include "ON_OFF.h"
#include "2803.h"
#include "adc.h"
#include "PWM.h"
#include "usart.h"
#include "usart3.h"
#include "RS485.h"
#include "function.h"
#include "Parse_frame_data.h"
#include "Execute_function.h"
#include "flag_function.h"

int main(void)
{	
  delay_init(); 
	
	uart_init(9600);
	RS485_init(115200);
	TIM1_PWM_Init(7200,50);
	
	LED_Init();
	ON_OFF_Init();
  OUT_Init();
	out_Init();	
  Debug_PIN_Init();
	PWM_PIN_INIT();
  Ready_Send();
	
	while(1)
		{	
			LED0=~LED0;	
			delay_ms(50);
			
			switch (function_flag)
			{
				case 1:base_function();delay_us(5);
					break;
				case 2:reset_function();delay_us(5);
					break;
				case 3:date_function();delay_us(5);
					break;
				default:
					break;
			}

				//очн╩	 
			if(ON_OFF1==1)
				{
					GPIO_ResetBits(GPIOC, GPIO_Pin_9);
				}
			if(ON_OFF2==1)
				{
					GPIO_ResetBits(GPIOC, GPIO_Pin_7);
				}	 
		}
}

