#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "key_test.h"
#include "wifi_test.h"
#include "lcd_test.h"
#include "tfcard_test.h"
#include "battery_test.h"
#include "internal_card_test.h"
#include "tvout_test.h"
#include "sound_test.h"
#include "debug.h"
#include "sdl_shape.h"

void test_loop(struct test_Parameters *test_para,int num)
{
    if(!strcmp(test_para->test_order[num],"lcd"))
    {
	debug_print("This is lcd test!\n");
	/*lcd_test();*/
	/*system("echo 50 > /proc/jz/lcd_backlight");*/
    }
    else if(!strcmp(test_para->test_order[num],"keypad"))
    {
	debug_print("This is keypad test!\n");
	/*key_test(test_para);*/
	/*sleep(5);*/
    }
    else if(!strcmp(test_para->test_order[num],"joystick"))
    {
	debug_print("This is joystick test!\n");
	/*joystick_test(test_para);*/
	/*sleep(5);*/
    }
    else if(!strcmp(test_para->test_order[num],"wifi"))
    {
	debug_print("This is wifi test!\n");
	/*wifi_test();*/
    }
    else if(!strcmp(test_para->test_order[num],"tfcard"))
    {
	debug_print("This is tfcard test!\n");
	/*tfcard_test();*/
    }
    else if(!strcmp(test_para->test_order[num],"battery_voltage"))
    {
	debug_print("This is battery valtage test!\n");
	/*battery_voltage_test();*/
    }
    else if(!strcmp(test_para->test_order[num],"internal_card"))
    {
	debug_print("This is internal_card test!\n");
	/*internal_card_test();*/
    }
    else if(!strcmp(test_para->test_order[num],"hdmi"))
    {
	debug_print("This is hdmi test!\n");
	/*hdmi_test();*/
    }
    else if(!strcmp(test_para->test_order[num],"avout"))
    {
	debug_print("This is avout test!\n");
	/*avout_test();*/
    }
    else if(!strcmp(test_para->test_order[num],"speaker"))
    {
	debug_print("This is speaker test!\n");
	/*speaker_test();*/
    }
    else if(!strcmp(test_para->test_order[num],"headphone"))
    {
	debug_print("This is headphone test!\n");
	/*hp_test();*/
    }
    else;
}

int main(void)
{
    struct test_Parameters test_para;
    int i;

    if (init_res(&test_para) < 0)
	exit(0);

    if (init_result_flag(&test_para) < 0)
	exit(0);

    if (init_sdl() < 0)
	exit(0);

    if(test_para.total_num > 0)
    {
	for(i = 0; i < test_para.total_num; i++)
	    test_loop(&test_para,i);
    }

    deinit_res(&test_para);
    return 0;
}
