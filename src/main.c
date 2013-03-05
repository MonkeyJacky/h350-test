#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "wejpconfig.h"

#define CONFIG_FILE "test.conf"
#define False -1
#define True 0
#define CONTINUE 1
#define TEST_ORDER "test_order"
#define MAX_SIZE 256
#define TOTAL_NUM "test_total_num"
#define deep_free(x)\
	free(x);\
	x = NULL;

static ConfigFile test_cf;
struct test_Parameters{
    int total_num;
    char** test_order;
    int* result_flag;
};

int test_cf_init(void)
{
    cfg_init_config_file_struct(&test_cf);
    if(CFG_ERROR == cfg_check_config_file(CONFIG_FILE))
	return CFG_ERROR;
    if(CFG_ERROR == cfg_read_config_file(&test_cf,CONFIG_FILE))
	return CFG_ERROR;

    return CFG_SUCCESS;
}

int init_res(struct test_Parameters *test_para)
{
    int i;

    if(test_cf_init() < 0)
    {
	return False;
    }

    test_para->total_num = cfg_get_key_value_to_int(test_cf,TOTAL_NUM);
    test_para->test_order = malloc(sizeof(char*) * test_para->total_num);

    for(i = 0; i < test_para->total_num; i++)
    {
	test_para->test_order[i] = malloc(sizeof(char) * MAX_SIZE);
    }

    get_array_from_conf_str(test_para->test_order,TEST_ORDER,test_cf);

    for(i = 0; i < test_para->total_num; i++)
    {
	printf("test order[%d] is %s\n",i,test_para->test_order[i]);
    }

    return True;
}

void deinit_res(struct test_Parameters *test_para)
{
    int i;

    for(i = 0; i<test_para->total_num; i++)
    {
	deep_free(test_para->test_order[i]);
    }
    deep_free(test_para->test_order);
}

int init_result_flag(struct test_Parameters *test_para)
{
    return True;
}

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

static int lcd_test()
{
    int i;
    char temp_command[MAX_SIZE] = {0};
    /*draw_color_bar(temp_fb,0);*/
    /*draw_color_bar(temp_fb,1);*/
    /*draw_color_bar(temp_fb,2);*/
    for(i = 20; i < 100; i=i+20)
    {
	sprintf(temp_command,"echo %d > /proc/jz/lcd_backlight",i);
	system(temp_command);
	usleep(10*1000);
    }

    /*return decision_warning_show();*/
    return 0;
}

void test_loop(struct test_Parameters *test_para,int num)
{
    if(!strcmp(test_para->test_order[num],"lcd"))
    {
	printf("This is lcd test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"keypad"))
    {
	printf("This is keypad test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"joystick"))
    {
	printf("This is joystick test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"wifi"))
    {
	printf("This is wifi test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"tfcard"))
    {
	printf("This is tfcard test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"internal_card"))
    {
	printf("This is internal_card test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"hdmi"))
    {
	printf("This is	hdmi test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"tvout"))
    {
	printf("This is tvout test!\n");
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

    if(test_para.total_num > 0)
    {
	for(i = 0; i < test_para.total_num; i++)
	    test_loop(&test_para,i);
    }

    return 0;
}
