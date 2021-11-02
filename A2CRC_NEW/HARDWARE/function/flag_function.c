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


void base_function(void)
{
	switch (base_function_flag)
  {
		case 0X11:__set_FAULTMASK(1); NVIC_SystemReset();break;
		/*****************************�ƶ�*****************************/
		case 0XE0:Push_L(0);break;//���� 
		case 0XE1:Push_L(1);break;//���� 
		case 0XD4:Push_L(2);break;//ֹͣ
		
		case 0XE7:Push_R(0);break;//���� 
		case 0XE8:Push_R(1);break;//���� 
		case 0XD9:Push_R(2);break;//ֹͣ	
		
		/*****************************���*****************************/
		case 0XE2:Amplitude_L(0);break;//����
		case 0XE3:Amplitude_L(1);break;//���� 
		case 0XD5:Amplitude_L(2);break;//ֹͣ
		
		case 0XE9:Amplitude_R(0);break;//����
		case 0XEA:Amplitude_R(1);break;//���� 
		case 0XDA:Amplitude_R(2);break;//ֹͣ

		/*****************************Ƶ��*****************************/
		//MOTOR_POWER
		case 0XDF:MOTOR_POWER(1);break;//�رյ�Դ 
				
		case 0XE5:PWM_L(0);MOTOR_L(0);break;//����������
		case 0XD8:MOTOR_L(1);break;//ֹͣ
		
		case 0XEC:PWM_R(0);MOTOR_R(0);break;//�Ҳ��������	
		case 0XDD:MOTOR_R(1);break;//ֹͣ	
		
		/*****************************����*****************************/
		case 0XD6:PWM_L(1);break;//�� �� +
		case 0XD7:PWM_L(2);break;//�� �� -
		case 0XEE:PWM_L(3);break;//�� ָ���ٶ�
			
		case 0XDB:PWM_R(1);break;//�� �� +
		case 0XDC:PWM_R(2);break;//�� �� -
		case 0XEF:PWM_R(3);break;//�� ָ���ٶ�	
		
    //����ͬ������
		case 0XCC:IN1=1;IN3=1;break;		
		
		//����ͬ��ֹͣ
		case 0XCD:
			IN1=0;IN3=0;
			pack_protocol(12);
			RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
			pack_protocol(12);
			RS485_Send_Data(RS485_TX_BUF,8);
		break;
					
		//��ͣ
		case 0XD3:STOP();break;

		default:
  		break;
  }
	clear_basefunction_flag();
}

void reset_function(void)
{
	switch (reset_function_flag)
  {
		case 0XDE:MOTOR_POWER(0);RESET_function();break;//������Դ 
		
		//�����λ
		case 0XE4:Amplitude_L(3);break;
		case 0XEB:Amplitude_R(3);break;
		
		//������λ
		case 0XCA:MOTOR_L(2);break;
		case 0XCB:MOTOR_R(2);break;
		
		//�ܸ�λ
		case 0XD2:RESET_function();break;
  	default:
  		break;
  }
}

void date_function(void)
{
	switch (date_function_flag)
  {
		
  	default:
  		break;
  }

}
