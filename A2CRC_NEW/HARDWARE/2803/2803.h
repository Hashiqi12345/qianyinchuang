#ifndef __OUT_H
#define __OUT_H			 
#include "sys.h"	 								  

#define OUT1 PGout(7)//PG7 
#define OUT2 PGout(6)//PG6 
#define OUT3 PGout(5)//PG5 
#define OUT4 PGout(4)//PG4 ������Դ 
#define OUT5 PGout(3)//PG3 ��ŷ�����1
#define OUT6 PGout(2)//PG2 ��ŷ�����2
#define OUT7 PDout(15)//PD15��ŷ����ң�1
#define OUT8 PDout(14)//PD16��ŷ����ң�2

#define out1_1 PCout(9)//PC9 С���������
#define out1_2 PCout(8)//PC8 С������󣩷�
#define out1_3 PCout(7)//PC7 С������ң���
#define out1_4 PCout(6)//PC6 С������ң���

void OUT_Init(void);//2803_0��ʼ��
void out_Init(void);//2803_1��ʼ��

#endif
