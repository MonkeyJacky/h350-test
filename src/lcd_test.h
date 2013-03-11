#ifndef _LCD_TEST_H_
#define _LCD_TEST_H_

#define BACKLIGHT_DEV "/proc/jz/lcd_backlight"

int lcd_test(void);

void adjust_backlight(int bklight_value);

#endif
