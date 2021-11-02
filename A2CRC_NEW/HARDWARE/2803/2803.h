#ifndef __OUT_H
#define __OUT_H			 
#include "sys.h"	 								  

#define OUT1 PGout(7)//PG7 
#define OUT2 PGout(6)//PG6 
#define OUT3 PGout(5)//PG5 
#define OUT4 PGout(4)//PG4 大电机电源 
#define OUT5 PGout(3)//PG3 电磁阀（左）1
#define OUT6 PGout(2)//PG2 电磁阀（左）2
#define OUT7 PDout(15)//PD15电磁阀（右）1
#define OUT8 PDout(14)//PD16电磁阀（右）2

#define out1_1 PCout(9)//PC9 小电机（左）正
#define out1_2 PCout(8)//PC8 小电机（左）反
#define out1_3 PCout(7)//PC7 小电机（右）正
#define out1_4 PCout(6)//PC6 小电机（右）反

void OUT_Init(void);//2803_0初始化
void out_Init(void);//2803_1初始化

#endif
