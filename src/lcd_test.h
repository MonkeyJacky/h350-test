#ifndef _LCD_TEST_H_
#define _LCD_TEST_H_

#include "init_parameters.h"
#define BACKLIGHT_DEV "/proc/jz/lcd_backlight"

int lcd_test(struct test_Parameters *test_para);

void adjust_backlight(int bklight_value);

#endif
