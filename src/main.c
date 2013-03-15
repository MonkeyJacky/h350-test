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
#include "init_parameters.h"

typedef struct{
    unsigned char* item_key_word;
    int (*item_testing_func)(struct test_Parameters *test_para);
}test_orders_;

test_orders_ test_order_array[] =
{
    {"lcd",lcd_test},
    {"keypad",key_test},
    {"joystick",joystick_test},
    {"wifi",wifi_test},
    {"tfcard",tfcard_test},
    {"bat-voltage",battery_voltage_test},
    {"internal_card",internal_card_test},
    {"hdmi",hdmi_test},
    {"avout",avout_test},
    {"speaker",speaker_test},
    {"headphone",hp_test},
};

void test_loop(struct test_Parameters *test_para,int num,int order_array_size)
{
    int i;

    for(i = 0;i < order_array_size; i++)
    {
	if(!strcmp(test_para->test_order[num],test_order_array[i].item_key_word))
	{
	    debug_print("This is %s test!\n",test_order_array[i].item_key_word);
	    test_para->result_flag[i] = test_order_array[i].item_testing_func(test_para);
	    PRINT_VALUE(test_para->result_flag[i],%d);
	}
    }
}

int main(void)
{
    struct test_Parameters test_para;
    int i;
    int test_order_array_size = 0;

    if (init_res(&test_para) < 0)
	exit(0);

    if (init_result_flag(&test_para) < 0)
	exit(0);

    if (init_sdl(&test_para) < 0)
	exit(0);

    init_key_pad();
    test_order_array_size = sizeof(test_order_array) / sizeof(test_orders_);
    flush_screen(1);
    if(test_para.total_num > 0 && test_para.total_num <= test_order_array_size)
    {
	for(i = 0; i < test_para.total_num; i++)
	    test_loop(&test_para,i,test_order_array_size);
    }
    else
    {
	debug_print("Get test order num error!\n");
    }

    deinit_key_pad();
    deinit_res(&test_para);
    deinit_sdl();
    flush_screen(0);
    return 0;
}