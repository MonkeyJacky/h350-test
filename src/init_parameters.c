#include <stdio.h>
#include <stdlib.h>
#include "init_parameters.h"
#include "debug.h"
#include "wejpconfig.h"

static ConfigFile test_cf;

static int test_cf_init(void)
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
    printf("test_para->total_num is %d\n",test_para->total_num);
    if(test_para->total_num < 0)
    {
	debug_print("get total_num error!\n");
	return False;
    }
    test_para->test_order = malloc(sizeof(char*) * test_para->total_num);

    test_para->key_num = cfg_get_key_value_to_int(test_cf,KEY_NUM);
    printf("test_para->key_num is %d\n",test_para->key_num);
    if(test_para->key_num < 0)
    {
	debug_print("get key_num error!\n");
	return False;
    }
    test_para->key_rect_array.x = malloc(sizeof(int) * test_para->key_num);
    test_para->key_rect_array.y = malloc(sizeof(int) * test_para->key_num);
    test_para->key_rect_array.w = malloc(sizeof(int) * test_para->key_num);
    test_para->key_rect_array.h = malloc(sizeof(int) * test_para->key_num);

    test_para->joykey_num = cfg_get_key_value_to_int(test_cf,JOY_KEY_NUM);
    if(test_para->joykey_num < 0)
    {
	debug_print("get joykey_num error!\n");
	return False;
    }
    test_para->joykey_rect_array.x = malloc(sizeof(int) * test_para->joykey_num);
    test_para->joykey_rect_array.y = malloc(sizeof(int) * test_para->joykey_num);
    test_para->joykey_rect_array.w = malloc(sizeof(int) * test_para->joykey_num);
    test_para->joykey_rect_array.h = malloc(sizeof(int) * test_para->joykey_num);

    for(i = 0; i < test_para->total_num; i++)
    {
	test_para->test_order[i] = malloc(sizeof(char) * MAX_SIZE);
    }

    if(get_array_from_conf_str(test_para->test_order,test_para->total_num,TEST_ORDER,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->key_rect_array.x,test_para->key_num,RECT_ARRAY_X,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->key_rect_array.y,test_para->key_num,RECT_ARRAY_Y,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->key_rect_array.w,test_para->key_num,RECT_ARRAY_W,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->key_rect_array.h,test_para->key_num,RECT_ARRAY_H,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->joykey_rect_array.x,test_para->joykey_num,JOYRECT_ARRAY_X,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->joykey_rect_array.y,test_para->joykey_num,JOYRECT_ARRAY_Y,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->joykey_rect_array.w,test_para->joykey_num,JOYRECT_ARRAY_W,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->joykey_rect_array.h,test_para->joykey_num,JOYRECT_ARRAY_H,test_cf) < 0)
	return False;

    for(i = 0; i < test_para->total_num; i++)
    {
	debug_print("test order[%d] is %s\n",i,test_para->test_order[i]);
    }

    for(i = 0; i < test_para->key_num; i++)
    {
	debug_print("key x is %d, key y is %d, key w is %d, key h is %d\n",test_para->key_rect_array.x[i],test_para->key_rect_array.y[i],test_para->key_rect_array.w[i],test_para->key_rect_array.h[i]);
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

    deep_free(test_para->key_rect_array.x);
    deep_free(test_para->key_rect_array.y);
    deep_free(test_para->key_rect_array.w);
    deep_free(test_para->key_rect_array.h);
    deep_free(test_para->joykey_rect_array.x);
    deep_free(test_para->joykey_rect_array.y);
    deep_free(test_para->joykey_rect_array.w);
    deep_free(test_para->joykey_rect_array.h);
}

int init_result_flag(struct test_Parameters *test_para)
{
    return True;
}


