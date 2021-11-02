#ifndef __Execute_function_H
#define __Execute_function_H
#include "sys.h"

extern u16 PWM_VALUE;

extern u8 function_flag;
extern u8 base_function_flag;
extern u8 reset_function_flag;
extern u8 date_function_flag;

void Execute_function_configure(void);
void flag_function_configure(void);
#endif
