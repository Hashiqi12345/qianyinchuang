#ifndef __Debug_PIN_H
#define __Debug_PIN_H
#include "sys.h"

#define IO1 PDout(4)//PD4 
#define IO2 PDout(5)//PD5 
#define IO3 PGout(9)//PG9
#define IO4 PGout(10)//PG10 
#define IO5 PGout(11)//PG11 
#define IO6 PGout(12)//PG12
#define IO7 PGout(13)//PD13
#define IO8 PGout(14)//PD14

void Debug_PIN_Init(void); //IO ≥ı ºªØ

#endif
