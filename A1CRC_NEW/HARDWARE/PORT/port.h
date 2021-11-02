#ifndef __PORT_H
#define	__PORT_H

#include "stm32f10x.h"

//2803_0
#define Y0 PGout(7)  //成角电机U
#define Y_0 PGout(6) //成角电机D
#define Y1 PGout(5)  //电磁阀1L
#define Y_1 PGout(4) //电磁阀1R
#define Y2 PGout(3)  //电磁阀2L
#define Y_2 PGout(2) //电磁阀2R
#define Y3 PDout(15) //电磁阀3L
#define Y_3 PDout(14)//电磁阀3R

//2803_1
#define Y4 PDout(11)
#define Y_4 PDout(10)
#define Y5 PDout(9)
#define Y_5 PDout(8)

void PORT_GPIO_Config(void);
void ON_OFF_Init(void);

#endif /* __PORT_H */
