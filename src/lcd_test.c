#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "debug.h"
#include "init_parameters.h"
#include "lcd_test.h"
#include "sdl_shape.h"
#include "key_test.h"
/*************************lcd test***************************/
static SDL_Color Bcolor = {0,0,0};
void flush_screen(int mode)
{
	unsigned char buff[32] = {0};
	FILE *brt_fd = fopen(FLUSH_DEVICE, "w");
	sprintf(buff,"%d",mode);
	fwrite(buff,32,1,brt_fd);
	//fflush(brt_fd);
	//fsync(brt_fd);
	fclose(brt_fd);
}

void draw_color_bar(unsigned int * temp_fb,unsigned int index,int width,int height)
{
    unsigned int *fb = temp_fb;
    unsigned int  i = 0;
    unsigned int j = 0;
    int k = 0;
    unsigned char offset[]={0,8,16};
    
    i = height / 10;
    if(i <= 0)
	return;
    unsigned int base_color = 0x09;
    while(i--)
    {
        j = width * 10;
        while(j--)
        {
            *fb++ = base_color << offset[index];
        }
        
        if(base_color + 0x9 <= 0xff)
            base_color += 0x9;
    }

    k = height % 10;
    if(k != 0)
    {
	j = width * k;
	while(j--)
	{
	    *fb++ = base_color << offset[index];
	}
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
    extern unsigned char *fb;
    test_words_show("Lcd and backlight test",Bcolor);

    PRINT_VALUE(test_para->screen_info.buffer_size,"%d");
    if(init_fb(test_para->screen_info.buffer_size) < 0)
    {
	draw_decision_pic(FAIL);
	return False;
    }

    for(i = 0; i < 3; i++) // 3 kind of colors.
    {
	draw_color_bar((unsigned int *)fb,i,test_para->screen_info.width,test_para->screen_info.height);
	sleep(2);
    }

    for(i = 20; i < 100; i += 20) // change backlight 20 values each.
	adjust_backlight(i);

    deinit_fb(test_para->screen_info.buffer_size);
    /*return decision_warning_show();*/
    return decision_loop();
}


