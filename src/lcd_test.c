#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "debug.h"
#include "init_parameters.h"
#include "lcd_test.h"
/*************************lcd test***************************/
static void draw_color_bar(unsigned int * temp_fb,unsigned int index)
{
    unsigned int *fb = temp_fb;
    unsigned int  i = 3;
    unsigned int j = 0;
    unsigned char offset[]={0,8,16};
    
    i = 27;
    unsigned int base_color = 0x09;
    while(i--)
    {
        j = 480 * 10;
        while(j--)
        {
            *fb++ = base_color << offset[index];
        }
        
        if(base_color + 0x9 <= 0xff)
            base_color += 0x9;
    }
    j = 480 * 2;
    while(j--)
    {
        *fb++ = base_color << offset[index];
    }
}

void adjust_backlight(int bklight_value)
{
    char temp_command[MAX_SIZE] = {0};

    sprintf(temp_command,"echo %d > %s",bklight_value,BACKLIGHT_DEV);
    system(temp_command);
    usleep(10*1000);
}

int lcd_test(struct test_Parameters *test_para)
{
    int i;
    unsigned int *temp_fb = NULL;
    //TODO framebuffer need be initted.

    for(i = 0; i < 3; i++) // 3 kind of colors.
	draw_color_bar(temp_fb,i);

    for(i = 20; i < 100; i += 20) // change backlight 20 values each.
	adjust_backlight(i);

    /*return decision_warning_show();*/
    return 0;
}


