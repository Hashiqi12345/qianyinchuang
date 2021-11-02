#ifndef __Parse_frame_data_H
#define __Parse_frame_data_H
#include "stdio.h"	
#include "sys.h" 

extern u8 PROTOCOL[8];

extern u8 PROTOCOL_ERROR;
extern u16 CRCtemp;
extern u16 RX_CRC;

extern u8 RS485_TX_BUF[8];
extern u8 FUNCION;
extern u16 TX_DATA;

extern u8 FUNCTIONG_FLAG;
extern u8 BREAK_FLAG;

#define PROTOCOL_ERROR1 0
#define PROTOCOL_ERROR2 1
#define PROTOCOL_ERROR3 2
#define PROTOCOL_OK 3

u8 protocol_frame(void);
void FEEDBACK_PROTOCOL_CHECK(void);

/***************
0:��ƵL 1:��ƵR 2:�����λL 3:�����λR 4:������λL 5:������λR
***************/
void pack_protocol(u8 num);//���һ֡���ݣ�������USART3SENDBUF

#endif
