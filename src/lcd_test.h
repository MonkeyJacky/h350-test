#ifndef _LCD_TEST_H_
#define _LCD_TEST_H_

#include "init_parameters.h"
#define BACKLIGHT_DEV "/proc/jz/lcd_backlight"
#define FLUSH_DEVICE "/proc/jz/lcd_flush"

int lcd_test(struct test_Parameters *test_para);

void adjust_backlight(int bklight_value);

void flush_screen(int mode);

void draw_color_bar(unsigned int * temp_fb,unsigned int index,int width,int height);

#endif
