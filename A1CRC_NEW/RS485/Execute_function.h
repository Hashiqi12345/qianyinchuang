#ifndef __Execute_function_H
#define __Execute_function_H
#include "sys.h"

extern u8 BREAK_FLAG;
extern u8 encode2_Reset;

extern u8 function_flag;
extern u8 base_function_flag;
extern u8 reset_function_flag;
extern u8 date_function_flag;
extern u8 setdata_function_flag;

void Execute_function_configure(void);
void flag_function_configure(void);

#endif
