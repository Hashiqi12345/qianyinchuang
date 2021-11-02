#include "Parse_frame_data.h"
#include "RS485.h"
#include "crc16.h"
#include "led.h"
#include "delay.h"
#include "Execute_function.h"
#include "encode.h"
#include "encode1.h"
#include "encode2.h"
#include "encode3.h"
#include "adc.h"
/*****************************
����һ֡����
*****************************/
u8 PROTOCOL_ERROR;
u16 CRCtemp;
u16 RX_CRC;

u8 protocol_frame(void)
	{		
		CRCtemp=GetCRC16(RS485_RX_BUF, 6);//����õ���CRCУ��ֵ
		RX_CRC=(RS485_RX_BUF[6]<<8)+RS485_RX_BUF[7];//���յ���CRCУ��ֵ
		
		if(RS485_RX_BUF[0]!=0XAF)return PROTOCOL_ERROR1;//ͷ֡У��
		else if(RS485_RX_BUF[2]!=0X00)return PROTOCOL_ERROR2;//0X00
		else if(RX_CRC!=CRCtemp)return PROTOCOL_ERROR3;//CRCУ��&&RX_CRC==CRCtemp
		else if(RS485_RX_BUF[0]==0XAF&&RS485_RX_BUF[1]==0XA1&&RS485_RX_BUF[2]==0X00)return PROTOCOL_OK;
	  return  0;
	}

/*********************************
���յ��������ݣ�������λ��
**************************************/	
u8 PROTOCOL_CHECK;
u8 FUNCTIONG_FLAG=0;
u8 BREAK_FLAG=0;
	
void FEEDBACK_PROTOCOL_CHECK(void)
	{
		u8 t,i;
		t=protocol_frame();
		switch (t)
    {
    	case 0:PROTOCOL_CHECK=0XAA;RS485_RX_BUF[0]=PROTOCOL_CHECK;RS485_RX_BUF[1]=0XA1;RS485_RX_BUF[2]=0X00;//ͷ֡У��
    		break;
    	case 1:PROTOCOL_CHECK=0XAB;RS485_RX_BUF[0]=PROTOCOL_CHECK;RS485_RX_BUF[1]=0XA1;RS485_RX_BUF[2]=0X00;//0X00
    		break;
			case 2:PROTOCOL_CHECK=0XAC;RS485_RX_BUF[0]=PROTOCOL_CHECK;RS485_RX_BUF[1]=0XA1;RS485_RX_BUF[2]=0X00;//CRCУ��
    		break;
			case 3:	
				for(i=0;i<RS485_RX_CNT;i++)PROTOCOL[i]=RS485_RX_BUF[i];
			  //RS485_Send_Data(RS485_RX_BUF,8);
			
			/***********************���ݲ���****************************/
//			  if(PROTOCOL[3]==0XCA){encode1_Send_Data();break;}
//				else if(PROTOCOL[3]==0XCB){encode2_Send_Data();break;}
//				else if(PROTOCOL[3]==0XCC){encode3_Send_Data();break;}
//				else if(PROTOCOL[3]==0XCD){encode_Send_Data();break;}
//				else if(PROTOCOL[3]==0XCE){ADC_Send_Data();break;}

				//BREAK_FLAG=1;//��ֵ1�����ϸ����ܴ���ѭ��������
			 // FUNCTIONG_FLAG=1;//����������ִ�й���
        break;
			
    	default:
    		break;
    }
	}
		


/*****************************
���һ֡���ݣ�������RS485_TX_BUF
ֻ�Դ����������ݴ��
****************************/
u8 FUNCION;
u8 TX_DATA1;
u8 TX_DATA2;
u8 RS485_TX_BUF[8];
	
void pack_protocol(u8 num)
	{
		u16 CRCtemp;
		
		RS485_TX_BUF[0]=0XAF;
		RS485_TX_BUF[1]=0XA1;
		RS485_TX_BUF[2]=0X00;
		delay_us(5);
		
		switch (num)
    {
    	case 1:
				RS485_TX_BUF[3]=0XB2;
			  RS485_TX_BUF[4]=Cencode[1];
			  RS485_TX_BUF[5]=Cencode[2];
    		break;
			
    	case 2:
				RS485_TX_BUF[3]=0XB5;
			  RS485_TX_BUF[4]=Bencode[1];
			  RS485_TX_BUF[5]=Bencode[2];
    		break;
			
			case 3:
				RS485_TX_BUF[3]=0XB8;
			  RS485_TX_BUF[4]=Xencode[1];
			  RS485_TX_BUF[5]=Xencode[2];
    		break;
			
			case 4:
				RS485_TX_BUF[3]=0XC2;
			  RS485_TX_BUF[4]=Gencode[0];
			  RS485_TX_BUF[5]=Gencode[1];
    		break;
			
		 case 5:
				RS485_TX_BUF[3]=0XC6;
			  RS485_TX_BUF[4]=Gencode[0];
			  RS485_TX_BUF[5]=Gencode[1];
    		break;
		 
		 case 6:
				RS485_TX_BUF[3]=0XC3;
			  RS485_TX_BUF[4]=0XFF;
			  RS485_TX_BUF[5]=ADCNS;
    		break;

		 case 7://�ɽǸ�λ
				RS485_TX_BUF[3]=0XB9;
			  RS485_TX_BUF[4]=0X01;
			  RS485_TX_BUF[5]=0X00;
    		break;
		 
		 case 8://�ɽ�STOP
				RS485_TX_BUF[3]=0XBC;
			  RS485_TX_BUF[4]=0X10;
			  RS485_TX_BUF[5]=0X00;
    		break;
		 
		 case 9://�ڽǸ�λ
				RS485_TX_BUF[3]=0XBA;
			  RS485_TX_BUF[4]=0X01;
			  RS485_TX_BUF[5]=0X00;
    		break;
		 
		 case 10://�ڽ�STOP
				RS485_TX_BUF[3]=0XBD;
			  RS485_TX_BUF[4]=0X10;
			  RS485_TX_BUF[5]=0X00;
    		break;
		 
		 case 11://���Ǹ�λ
				RS485_TX_BUF[3]=0XBB;
			  RS485_TX_BUF[4]=0X01;
			  RS485_TX_BUF[5]=0X00;
    		break;
		 
		 case 12://����STOP
				RS485_TX_BUF[3]=0XBE;
			  RS485_TX_BUF[4]=0X10;
			  RS485_TX_BUF[5]=0X00;
    		break;
		 
		 case 13://ǣ����λ
				RS485_TX_BUF[3]=0XC8;
			  RS485_TX_BUF[4]=0X01;
			  RS485_TX_BUF[5]=0X00;
    		break;
		 
		 case 14://ǣ��STOP
				RS485_TX_BUF[3]=0XC9;
			  RS485_TX_BUF[4]=0X10;
			  RS485_TX_BUF[5]=0X00;
    		break;
		 
		 case 15://�ֶ��ڽǸ�λ
				RS485_TX_BUF[3]=0XD2;
			  RS485_TX_BUF[4]=0X01;
			  RS485_TX_BUF[5]=0X00;
    		break;
		 
		 case 16://�ֶ����Ǹ�λ
				RS485_TX_BUF[3]=0XD4;
			  RS485_TX_BUF[4]=0X01;
			  RS485_TX_BUF[5]=0X00;
    		break;

		 case 17://�ֶ��ɽǸ�λ
				RS485_TX_BUF[3]=0XD6;
			  RS485_TX_BUF[4]=0X01;
			  RS485_TX_BUF[5]=0X00;
    		break;		 
		 
		 default:
    		break;
    }
		
		CRCtemp = GetCRC16(RS485_TX_BUF, 6);//ǰ��λCRCУ��
	
   RS485_TX_BUF[6]=(CRCtemp >> 8);//���CRCh
		RS485_TX_BUF[7]=CRCtemp;//����CRCl
    delay_us(5);
	}	
		