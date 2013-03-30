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
#include "sdl_interface.h"
#include "init_parameters.h"

typedef struct{
    char* item_key_word;
    int (*item_testing_func)(struct test_Parameters *test_para);
}test_orders_;

test_orders_ test_order_array[] =
{
    {"Lcd",lcd_test},
    {"Keypad",key_test},
    {"Joystick",joystick_test},
    {"Wifi",wifi_test},
    {"Tfcard",tfcard_test},
    {"Bat-Voltage",battery_voltage_test},
    {"Internal_Card",internal_card_test},
    {"Hdmi",hdmi_test},
    {"Avout",avout_test},
    {"Speaker",speaker_test},
    {"Headphone",hp_test},
};

void test_loop(struct test_Parameters *test_para,int num,int order_array_size)
{
    int i;

    for(i = 0;i < order_array_size; i++)
    {
	if(!strcmp(test_para->test_order[num],test_order_array[i].item_key_word))
	{
	    debug_print("This is %s test!\n",test_order_array[i].item_key_word);
	    test_para->result_flag[num] = test_order_array[i].item_testing_func(test_para);
	    store_result_flag(test_para,num);
	    debug_print("test_para->result_flag[%d] is %d\n",num,test_para->result_flag[num]);
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

#ifdef H350
    init_key_pad();
#endif
    test_order_array_size = sizeof(test_order_array) / sizeof(test_orders_);
#ifdef H350
    flush_screen(1);
#endif
    if(test_para.total_num > 0 && test_para.total_num <= test_order_array_size)
    {
	for(i = 0; i < test_para.total_num; i++)
	    test_loop(&test_para,i,test_order_array_size);
    }
    else
    {
	debug_print("Get test order num error!\n");
    }

    result_show(&test_para);
    sleep(5);

#ifdef H350
    deinit_key_pad();
#endif
    deinit_res(&test_para);
    deinit_result_conf();
    deinit_sdl();
#ifdef H350
    flush_screen(0);
#endif
    return 0;
}
