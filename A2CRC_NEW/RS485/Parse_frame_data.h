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
0:测频L 1:测频R 2:振幅复位L 3:振幅复位R 4:大电机复位L 5:大电机复位R
***************/
void pack_protocol(u8 num);//打包一帧数据，储存在USART3SENDBUF

#endif
