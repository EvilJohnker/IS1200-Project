#ifndef _TEMP_HELPERS_H
#define _TEMP_HELPERS_H

#include <stdint.h>

void period_calc(int period, float *avg, float *minp, float *maxp);
float fixed_to_float(uint16_t fixed);
void user_selection(void);
float read_temperature(void);
void float_to_string(float f);
float celsius_to_farenheit(float celsius);
float celsius_to_kelvin(float celsius);
int get_btns(void);
void get_info(void);
void period_unit_char(void);

#endif