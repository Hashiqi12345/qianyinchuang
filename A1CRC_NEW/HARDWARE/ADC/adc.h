#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

void Adc_Init(void);
void ADC_NS(void);
void ADC_Send_Data(void);

extern u8 ADCNS;
u16  Get_Adc(u8 ch); 
u16  Get_Adc_Average(u8 ch,u8 times); 


#endif 
