#ifndef __flag_function_H__
#define __flag_function_H__

#include "stm32f10x.h"

static u8 updateflag;
static u8 update_base_flag;
static u8 update_reset_flag;
static u8 update_date_flag;

void clear_basefunction_flag(void);
void clear_resetfunction_flag(void);
void clear_datefunction_flag(void);
void clear_setdatafunction_flag(void);

void update_flag(void);

void base_function(void);
void reset_function(void);
void date_function(void);
void setdata_function(void);

#endif
