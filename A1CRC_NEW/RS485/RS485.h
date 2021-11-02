#ifndef __RS485_H
#define __RS485_H

#include "stdio.h"	
#include "sys.h" 
	  	
extern u8 RS485_RX_BUF[64]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度
extern u8 PROTOCOL[64];	

void RS485_init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void Ready_Send(void);
void RS485_Send_HalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
#endif	
