#ifndef __RS485_H
#define __RS485_H

#include "stdio.h"	
#include "sys.h" 
	  	
extern u8 RS485_RX_BUF[64]; 		//���ջ���,���64���ֽ�
extern u8 RS485_RX_CNT;   			//���յ������ݳ���
extern u8 PROTOCOL[64];	

void RS485_init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void Ready_Send(void);
void RS485_Send_HalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
#endif	
