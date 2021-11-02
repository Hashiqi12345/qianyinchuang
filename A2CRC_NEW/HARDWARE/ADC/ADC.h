#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

void Adc_Init(void);
u16 Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 

extern u16 adc_DCL;
extern u16 adc_DCR;

extern u16 temp_DCL;
extern u16 temp_DCR;
	
void ADC_DCL(void);
void ADC_DCR(void);
//void ADC_DCL_VALUE(void);
#endif 
