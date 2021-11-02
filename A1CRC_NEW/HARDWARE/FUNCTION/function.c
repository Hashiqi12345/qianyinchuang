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
���� �� ������DISTANCE_function
������˵����: 
***********************/
//ǣ������DISTANCE

void DISTANCE_function(u8 num)
{
	switch (num)
  {
		//ǣ��
  	case 0:
			GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		  GPIO_SetBits(GPIOD,GPIO_Pin_15);
//		  encode();
		  encode_limit(mode);
		break;
		
		//�ظ�
		case 1:
			GPIO_ResetBits(GPIOD,GPIO_Pin_15);
		  GPIO_SetBits(GPIOD,GPIO_Pin_14);
//		  encode();
		  encode_limit(mode);
		break;
		
  	case 2://ֹͣ
			GPIO_SetBits(GPIOD,GPIO_Pin_14|GPIO_Pin_15);
			pack_protocol(14);
			RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
		  encode_Send_Data();
		break;
		
		case 3:encode();break;
		
		case 4://ǣ����λ
			encode_RESET();
			delay_us(5);break;
		
		default:break;
  }
}


/***********************
���� �� ������ANGLE1_function
������˵����: 
***********************/
//�ɽ�ANGLE1

void ANGLE1_function(u8 num1)
{ 
	switch (num1)
  {
		//�����˶�
  	case 0:
			GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		  GPIO_SetBits(GPIOG,GPIO_Pin_6);
		  //encode1();
		  encode1_limit();
		break;//�ɽǵ������
		//�����˶�
		case 1:
			GPIO_ResetBits(GPIOG,GPIO_Pin_6);
		  GPIO_SetBits(GPIOG,GPIO_Pin_7);
		  //encode1();
		  encode1_limit();
		break;//�ɽǵ������
		
  	case 2:
			GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7);
		  pack_protocol(8);
		  RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
		  encode1_Send_Data();
		break;//�ɽǵ��ֹͣ
		
    case 3:encode1();break;
		case 4://�ɽǸ�λ
			encode1_RESET();
		 
			delay_us(5);
		break;
		
		default:break;
	}
}

/***********************
���� �� ������ANGLE2_function
������˵����: 
***********************/
//�ڽ�ANGLE2

void ANGLE2_function(u8 num2)
{
	switch (num2)
  {
		//�����˶�
  	case 0:
			GPIO_ResetBits(GPIOG,GPIO_Pin_5);
		  GPIO_SetBits(GPIOG,GPIO_Pin_4);
		  //encode2();
		  encode2_limit();
		break;//����
		//�����˶�
		case 1:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);
		  GPIO_SetBits(GPIOG,GPIO_Pin_5);
		  encode2();
		  encode2_limit();
		break;//����
		
  	case 2:
			GPIO_SetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5);
		  pack_protocol(10);
		  RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
		  encode2_Send_Data();
		break;//ֹͣ
		
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
���� �� ������ANGLE3_function
������˵����: 
***********************/
//����ANGLE3

void ANGLE3_function(u8 num3)
{ 
	switch (num3)
  {
		//��
  	case 0:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);
		  GPIO_SetBits(GPIOG,GPIO_Pin_2);
		 // encode3();
		  encode3_limit();
		break;//����
		//��
		case 1:
			GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		  GPIO_SetBits(GPIOG,GPIO_Pin_3);
		 // encode3();
		  encode3_limit();
		break;//����
		
  	case 2:
			GPIO_SetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3);
      pack_protocol(12);
		  RS485_Send_Data(RS485_TX_BUF,8);
			delay_us(5);
      encode3_Send_Data();		
		break;//ֹͣ
    case 3:encode3();break;
		case 4://���Ǹ�λ
			encode3_RESET();
		break;
		
		default:break;  
	}
}

/***********************
���� �� ������STOP��ͣ
������˵����: 
***********************/
void STOP(void)
	{
		DISTANCE_function(2);encode_Send_Data();delay_us(5);
		ANGLE1_function(2);encode1_Send_Data();delay_us(5);
		ANGLE2_function(2);encode2_Send_Data();delay_us(5);
		ANGLE3_function(2);encode3_Send_Data();delay_us(5);
}
/***********************
���� �� ������RESET��λ
������˵����: 
***********************/
void RESET_PLACE(void)
	{
	 encode_RESET();encode_Send_Data();
	 encode1_RESET();encode1_Send_Data();
	 encode2_RESET();encode2_Send_Data();
	 encode3_RESET();encode3_Send_Data();
	}

/***********************
������λ���趨������ִ�й���
*��դ��
*�ɽǱ�����
*�ڽǱ�����
*���Ǳ�����
***********************/
extern u8 PROTOCOL[64];
	
void ENCODE_function(u8 num)
	{
		u8 DATA1,DATA2;
		u8 encode_temp_DATA;
		u8 encode1_temp_DATA1,encode1_temp_DATA2;
		u8 encode2_temp_DATA1,encode2_temp_DATA2;
		u8 encode3_temp_DATA1,encode3_temp_DATA2;
		
		DATA1=PROTOCOL[4];//��ȡ��λ�����õ�����
		DATA2=PROTOCOL[5];//��ȡ��λ�����õ�λ�ơ��Ƕ�����
		
		delay_us(5);
		
		switch (num)
			{
			  case 0://�ɽǱ�����
					encode1();
				  encode1_temp_DATA1=Cencode[1];//��ǰ����
				  encode1_temp_DATA2=Cencode[2];//��ǰ�Ƕ�
				
				  if((DATA1==0&&DATA2>=0x0B)||(DATA1==1&&DATA2>=0x03))break;
					
				  if(DATA1!=encode1_temp_DATA1||DATA2!=encode1_temp_DATA2)
						{
							encode1();
				      encode1_temp_DATA1=Cencode[1];//ʵ������
				      encode1_temp_DATA2=Cencode[2];//ʵ�ʽǶ�
							
						if((encode1_temp_DATA1==0&&encode1_temp_DATA2<=0x0B)
							||(encode1_temp_DATA1==1&&encode1_temp_DATA2<=0x03))//��λ
						{
							if(DATA1!=encode1_temp_DATA1&&DATA2!=encode1_temp_DATA2)//��ͬ���� ��ͬ�Ƕ�
								{
									if(DATA1==0)ANGLE1_function(1);
									else if(DATA1==1)ANGLE1_function(0);
								}
								
							else if(DATA1==0&&DATA1==encode1_temp_DATA1&&DATA2!=encode1_temp_DATA2)//ͬ�ϲ� ��ͬ�Ƕ�
								{
									if(DATA2>encode1_temp_DATA2)ANGLE1_function(1);
									else if(DATA2<encode1_temp_DATA2)ANGLE1_function(0);
								}
								
							else if(DATA1==1&&DATA1==encode1_temp_DATA1&&DATA2!=encode1_temp_DATA2)//ͬ�²� ��ͬ�Ƕ�
								{
									if(DATA2>encode1_temp_DATA2)ANGLE1_function(0);
									else if(DATA2<encode1_temp_DATA2)ANGLE1_function(1);
								}
						}
					} 
						else if(DATA1==encode1_temp_DATA1&&DATA2==encode1_temp_DATA2)//ͬ���� ͬ�Ƕ�
							{
								ANGLE1_function(2);
								clear_setdatafunction_flag();
								DATA1=0;DATA2=0;
							}	 	
      		break;
				
      	case 1://�ڽǱ�����
					encode2();
				  encode2_temp_DATA1=Bencode[1];//��ǰ����
				  encode2_temp_DATA2=Bencode[2];//��ǰ�Ƕ�
				
				  if(DATA2>=0X0F)break;
				
					if(DATA1!=encode2_temp_DATA1||DATA2!=encode2_temp_DATA2)
					{
							encode2();
				      encode2_temp_DATA1=Bencode[1];//ʵ������
				      encode2_temp_DATA2=Bencode[2];//ʵ�ʽǶ�
							
						if(encode2_temp_DATA2<0X0F)
						{
							if(DATA1!=encode2_temp_DATA1&&DATA2!=encode2_temp_DATA2)//��ͬ���� ��ͬ�Ƕ�
								{
									if(DATA1==0)ANGLE2_function(1);
									else if(DATA1==1)ANGLE2_function(0);
								}
								
							else if(DATA1==0&&DATA1==encode2_temp_DATA1&&DATA2!=encode2_temp_DATA2)//ͬ��� ��ͬ�Ƕ�
								{
									if(DATA2>encode2_temp_DATA2)ANGLE2_function(1);
									else if(DATA2<encode2_temp_DATA2)ANGLE2_function(0);
								}
								
							else if(DATA1==1&&DATA1==encode2_temp_DATA1&&DATA2!=encode2_temp_DATA2)//ͬ�Ҳ� ��ͬ�Ƕ�
								{
									if(DATA2>encode2_temp_DATA2)ANGLE2_function(0);
									else if(DATA2<encode2_temp_DATA2)ANGLE2_function(1);
								}
						}
					}
					else if(DATA1==encode2_temp_DATA1&&DATA2==encode2_temp_DATA2)//ͬ���� ͬ�Ƕ�
					{
						ANGLE2_function(2);
						delay_us(5);
						clear_setdatafunction_flag();
						DATA1=0;DATA2=0;
					}
      		break;
				
        case 2://���Ǳ�����
					encode3();
				  encode3_temp_DATA1=Xencode[1];//��ǰ����
				  encode3_temp_DATA2=Xencode[2];//��ǰ�Ƕ�
				
				  if(DATA2>=0X0B)break;
				
					if(DATA1!=encode3_temp_DATA1||DATA2!=encode3_temp_DATA2)
					{
							encode3();
				      encode3_temp_DATA1=Xencode[1];//ʵ������
				      encode3_temp_DATA2=Xencode[2];//ʵ�ʽǶ�
							
						if(encode3_temp_DATA2<0X0A)
						{
							if(DATA1!=encode3_temp_DATA1&&DATA2!=encode3_temp_DATA2)//��ͬ���� ��ͬ�Ƕ�
								{
									if(DATA1==0)ANGLE3_function(1);
									else if(DATA1==1)ANGLE3_function(0);
								}
								
							else if(DATA1==0&&DATA1==encode3_temp_DATA1&&DATA2!=encode3_temp_DATA2)//ͬ��� ��ͬ�Ƕ�
								{
									if(DATA2>encode3_temp_DATA2)ANGLE3_function(1);
									else if(DATA2<encode3_temp_DATA2)ANGLE3_function(0);
								}
								
							else if(DATA1==1&&DATA1==encode3_temp_DATA1&&DATA2!=encode3_temp_DATA2)//ͬ�Ҳ� ��ͬ�Ƕ�
								{
									if(DATA2>encode3_temp_DATA2)ANGLE3_function(0);
									else if(DATA2<encode3_temp_DATA2)ANGLE3_function(1);
								}
						}
					}
					else if(DATA1==encode3_temp_DATA1&&DATA2==encode3_temp_DATA2)//ͬ���� ͬ�Ƕ�
						{
							ANGLE3_function(2);
							delay_us(5);
							clear_setdatafunction_flag();
							DATA1=0;DATA2=0;
						}
      		break;
				
        case 3://��դ��  û��ʹ�ø�ѡ��
					encode();
				  encode_temp_DATA=distance;;//��ǰ����
				
	//			  if(DATA2>0X46)break;
				
					if(DATA2!=encode_temp_DATA)
						{
							encode();
				      encode_temp_DATA=distance;//ʵ�ʾ���
							
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
	
	BDATA1=PROTOCOL[4];//��ȡ��λ�����õ�����
	BDATA2=PROTOCOL[5];//��ȡ��λ�����õ�λ�ơ��Ƕ�����
	
	encode2();
	encode2_temp_DATA1=Bencode[1];//��ǰ����
	encode2_temp_DATA2=Bencode[2];//��ǰ�Ƕ�

	if(BDATA2>=0X0F){clear_setdatafunction_flag();}

	while(BDATA1!=encode2_temp_DATA1||BDATA2!=encode2_temp_DATA2)
		{
			if(BREAK_FLAG ==1 )break;//�ⲿ����ֵ1����ǿ�������ж�
			
			encode2();
			encode2_temp_DATA1=Bencode[1];//ʵ������
			encode2_temp_DATA2=Bencode[2];//ʵ�ʽǶ�
			
		if(encode2_temp_DATA2<0X0F)
		{
			if(BDATA1!=encode2_temp_DATA1&&BDATA2!=encode2_temp_DATA2)//��ͬ���� ��ͬ�Ƕ�
				{
					if(BDATA1==0)ANGLE2_function(1);
					else if(BDATA1==1)ANGLE2_function(0);
				}
				
			else if(BDATA1==0&&BDATA1==encode2_temp_DATA1&&BDATA2!=encode2_temp_DATA2)//ͬ��� ��ͬ�Ƕ�
				{
					if(BDATA2>encode2_temp_DATA2)ANGLE2_function(1);
					else if(BDATA2<encode2_temp_DATA2)ANGLE2_function(0);
				}
				
			else if(BDATA1==1&&BDATA1==encode2_temp_DATA1&&BDATA2!=encode2_temp_DATA2)//ͬ�Ҳ� ��ͬ�Ƕ�
				{
					if(BDATA2>encode2_temp_DATA2)ANGLE2_function(0);
					else if(BDATA2<encode2_temp_DATA2)ANGLE2_function(1);
				}
				
			else if(BDATA1==encode2_temp_DATA1&&BDATA2==encode2_temp_DATA2)break;//ͬ���� ͬ�Ƕ�
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
	
	XDATA1=PROTOCOL[4];//��ȡ��λ�����õ�����
	XDATA2=PROTOCOL[5];//��ȡ��λ�����õ�λ�ơ��Ƕ�����
	
	encode3();
	encode3_temp_DATA1=Cencode[1];//��ǰ����
	encode3_temp_DATA2=Cencode[2];//��ǰ�Ƕ�

	if(XDATA2>=0X0B){clear_setdatafunction_flag();}

	while(XDATA1!=encode3_temp_DATA1||XDATA2!=encode3_temp_DATA2)
		{
			if(BREAK_FLAG == 1)break;//�ⲿ����ֵ1����ǿ�������ж�
			
			encode3();
			encode3_temp_DATA1=Xencode[1];//ʵ������
			encode3_temp_DATA2=Xencode[2];//ʵ�ʽǶ�
			
		if(encode3_temp_DATA2<0X0B)
		{
			if(XDATA1!=encode3_temp_DATA1&&XDATA2!=encode3_temp_DATA2)//��ͬ���� ��ͬ�Ƕ�
				{
					if(XDATA1==0)ANGLE3_function(1);
					else if(XDATA1==1)ANGLE3_function(0);
				}
				
			else if(XDATA1==0&&XDATA1==encode3_temp_DATA1&&XDATA2!=encode3_temp_DATA2)//ͬ��� ��ͬ�Ƕ�
				{
					if(XDATA2>encode3_temp_DATA2)ANGLE3_function(1);
					else if(XDATA2<encode3_temp_DATA2)ANGLE3_function(0);
				}
				
			else if(XDATA1==1&&XDATA1==encode3_temp_DATA1&&XDATA2!=encode3_temp_DATA2)//ͬ�Ҳ� ��ͬ�Ƕ�
				{
					if(XDATA2>encode3_temp_DATA2)ANGLE3_function(0);
					else if(XDATA2<encode3_temp_DATA2)ANGLE3_function(1);
				}
				
			else if(XDATA1==encode3_temp_DATA1&&XDATA2==encode3_temp_DATA2)break;//ͬ���� ͬ�Ƕ�
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
	
	GDATA=PROTOCOL[5];//��ȡ��λ�����õ�λ�ơ��Ƕ�����
	encode();
	encode_temp_DATA=Gencode[1];;//��ǰ����

  if(GDATA>=0X46){clear_setdatafunction_flag();}
				
	while(GDATA!=encode_temp_DATA)
		{	
			if(BREAK_FLAG == 1)break;//�ⲿ����ֵ1����ǿ�������ж�		
			
			encode();
			encode_temp_DATA=Gencode[1];//ʵ�ʾ���
			
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
//���� �� ������AngleAreaData_Save
//������˵����: ���Ƕȱ仯��λ��������Ϣ����FLASH��
//***********************/	
//u16 AngleAreaData[3];	
//#define SIZE sizeof(AngleAreaData)		//���鳤��		
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
//���� �� ������������ʼ��λvoid Initial_Reset(void)
//������˵����: �����󣬶�ȡFLASH���������һ�ε���̬��Ϣ��
//		          ǣ�������ݸ���Ϣ�Դ����������ĸ�λ
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
		