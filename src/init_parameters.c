#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "init_parameters.h"
#include "debug.h"
#include "wejpconfig.h"

static ConfigFile test_cf;
static int ipu_fb = -1;
unsigned char* fb = NULL;

int init_fb(int buffer_size)
{
    ipu_fb = open(FRAME_BUFFER_DEV, O_RDWR);
    if(ipu_fb < 0)
    {
	debug_print ("fb init error!\n");
	return False;
    }

    fb = (unsigned char*)mmap(NULL,buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, ipu_fb, 0);
    if(!fb)
	return False;

    return True;
}

void deinit_fb(int buffer_size)
{
    if(fb)
	munmap(fb,buffer_size);

    if(ipu_fb){
	close(ipu_fb);
	ipu_fb = -1;
    }
}


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
    debug_print("test_para->total_num is %d\n",test_para->total_num);
    if(test_para->total_num < 0)
    {
	debug_print("get total_num error!\n");
	return False;
    }
    test_para->test_order = malloc(sizeof(char*) * test_para->total_num);
    test_para->result_flag = malloc(sizeof(char*) * test_para->total_num);

    test_para->key_num = cfg_get_key_value_to_int(test_cf,KEY_NUM);
    debug_print("test_para->key_num is %d\n",test_para->key_num);
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

    test_para->gsensor_direction_num = cfg_get_key_value_to_int(test_cf,GSENSOR_DIRECTION_NUM);
    debug_print("test_para->gsensor_direction_num is %d\n",test_para->gsensor_direction_num);
    if(test_para->gsensor_direction_num < 0)
    {
	debug_print("get gsensor_direction_num error!\n");
	return False;
    }
    test_para->gsensor_rect_array.x = malloc(sizeof(int) * test_para->gsensor_direction_num);
    test_para->gsensor_rect_array.y = malloc(sizeof(int) * test_para->gsensor_direction_num);
    test_para->gsensor_rect_array.w = malloc(sizeof(int) * test_para->gsensor_direction_num);
    test_para->gsensor_rect_array.h = malloc(sizeof(int) * test_para->gsensor_direction_num);

    for(i = 0; i < test_para->total_num; i++)
    {
	test_para->test_order[i] = malloc(sizeof(char) * MAX_SIZE);
	memset(test_para->test_order[i],0,MAX_SIZE);
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
    if(get_array_from_conf_int(test_para->gsensor_rect_array.x,test_para->gsensor_direction_num,GSENSOR_RECT_X,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->gsensor_rect_array.y,test_para->gsensor_direction_num,GSENSOR_RECT_Y,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->gsensor_rect_array.w,test_para->gsensor_direction_num,GSENSOR_RECT_W,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->gsensor_rect_array.h,test_para->gsensor_direction_num,GSENSOR_RECT_H,test_cf) < 0)
	return False;

    for(i = 0; i < test_para->total_num; i++)
    {
	debug_print("test order[%d] is %s\n",i,test_para->test_order[i]);
    }

    for(i = 0; i < test_para->key_num; i++)
    {
	debug_print("key x is %d, key y is %d, key w is %d, key h is %d\n",test_para->key_rect_array.x[i],test_para->key_rect_array.y[i],test_para->key_rect_array.w[i],test_para->key_rect_array.h[i]);
    }

    test_para->screen_info.width = 
	cfg_get_key_value_to_int(test_cf,SCREEN_WIDTH);
    test_para->screen_info.height = 
	cfg_get_key_value_to_int(test_cf,SCREEN_HEIGHT);
    test_para->screen_info.bits_per_pixel = 
	cfg_get_key_value_to_int(test_cf,SCREEN_BITS_PER_PIXEL);
    test_para->screen_info.buffer_size =
	test_para->screen_info.width * test_para->screen_info.height * test_para->screen_info.bits_per_pixel / 8;
    if( (test_para->screen_info.width |
	    test_para->screen_info.height |
	    test_para->screen_info.bits_per_pixel |
	    test_para->screen_info.buffer_size) < 0 )
    {
	debug_print("get screen info error!\n");
	return False;
    }

    test_para->hdmi_info.width = 
	cfg_get_key_value_to_int(test_cf,HDMI_SCREEN_WIDTH);
    test_para->hdmi_info.height = 
	cfg_get_key_value_to_int(test_cf,HDMI_SCREEN_HEIGHT);
    test_para->hdmi_info.buffer_size = 
	test_para->hdmi_info.width * test_para->hdmi_info.height * test_para->screen_info.bits_per_pixel / 8;
    if( (test_para->hdmi_info.width |
		test_para->hdmi_info.height |
		test_para->hdmi_info.buffer_size) < 0 )
    {
	debug_print("get hdmi info error!\n");
	return False;
    }

    test_para->avout_info.width = 
	cfg_get_key_value_to_int(test_cf,AVOUT_SCREEN_WIDTH);
    test_para->avout_info.height = 
	cfg_get_key_value_to_int(test_cf,AVOUT_SCREEN_HEIGHT);
    test_para->avout_info.buffer_size = 
	test_para->avout_info.width * test_para->avout_info.height * test_para->screen_info.bits_per_pixel / 8;
    if( (test_para->avout_info.width |
		test_para->avout_info.height |
		test_para->avout_info.buffer_size) < 0 )
    {
	debug_print("get avout info error!\n");
	return False;
    }

    test_para->result_view_para.array_size =
	cfg_get_key_value_to_int(test_cf,RESULT_VIEW_NUM);
    PRINT_VALUE(test_para->result_view_para.array_size,%d);
    if(test_para->result_view_para.array_size < 0)
    {
	debug_print("result array size init error!\n");
	return False;
    }
    test_para->result_view_para.result_words =
	malloc(sizeof(char *) * test_para->result_view_para.array_size);
    test_para->result_view_para.fail_color =
	malloc(sizeof(int) * test_para->result_view_para.array_size);
    test_para->result_view_para.pass_color =
	malloc(sizeof(int) * test_para->result_view_para.array_size);
    test_para->result_view_para.untest_color =
	malloc(sizeof(int) * test_para->result_view_para.array_size);

    for(i = 0; i < test_para->result_view_para.array_size; i++)
    {
	test_para->result_view_para.result_words[i] =
	    malloc(sizeof(char) * MAX_SIZE);
	memset(test_para->result_view_para.result_words[i],0,MAX_SIZE);
    }

    if(get_array_from_conf_str(test_para->result_view_para.result_words,test_para->result_view_para.array_size,TEST_RESULT_WORD,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->result_view_para.fail_color,test_para->result_view_para.array_size,FAIL_COLOR,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->result_view_para.pass_color,test_para->result_view_para.array_size,PASS_COLOR,test_cf) < 0)
	return False;
    if(get_array_from_conf_int(test_para->result_view_para.untest_color,test_para->result_view_para.array_size,UNTEST_COLOR,test_cf) < 0)
	return False;

    for(i = 0; i < test_para->result_view_para.array_size; i++)
	debug_print("test_para->result_view_para.result_words[%d] is %s\n",i,test_para->result_view_para.result_words[i]);
    for(i = 0; i < test_para->result_view_para.array_size; i++)
	debug_print("test_para->result_view_para.fail_color[%d] is %d\n",i,test_para->result_view_para.fail_color[i]);
    for(i = 0; i < test_para->result_view_para.array_size; i++)
	debug_print("test_para->result_view_para.pass_color[%d] is %d\n",i,test_para->result_view_para.pass_color[i]);
    for(i = 0; i < test_para->result_view_para.array_size; i++)
	debug_print("test_para->result_view_para.untest_color[%d] is %d\n",i,test_para->result_view_para.untest_color[i]);

    return True;
}

void deinit_res(struct test_Parameters *test_para)
{
    int i;

    for(i = 0; i<test_para->total_num; i++)
	deep_free(test_para->test_order[i]);
    deep_free(test_para->test_order);

    //TODO there is a free error.
    /*for(i = 0; i<test_para->result_view_para.array_size; i++)*/
	/*deep_free(test_para->result_view_para.result_words[i]);*/
    /*deep_free(test_para->result_view_para.result_words);*/

    deep_free(test_para->result_flag);
    deep_free(test_para->key_rect_array.x);
    deep_free(test_para->key_rect_array.y);
    deep_free(test_para->key_rect_array.w);
    deep_free(test_para->key_rect_array.h);
    deep_free(test_para->joykey_rect_array.x);
    deep_free(test_para->joykey_rect_array.y);
    deep_free(test_para->joykey_rect_array.w);
    deep_free(test_para->joykey_rect_array.h);
    deep_free(test_para->gsensor_rect_array.x);
    deep_free(test_para->gsensor_rect_array.y);
    deep_free(test_para->gsensor_rect_array.w);
    deep_free(test_para->gsensor_rect_array.h);
    deep_free(test_para->result_view_para.fail_color);
    deep_free(test_para->result_view_para.pass_color);
    deep_free(test_para->result_view_para.untest_color);

    cfg_free_config_file_struct(&test_cf);
}

static ConfigFile result_cf;
static int result_cf_init(void)
{
    int ret = CFG_SUCCESS;
    cfg_init_config_file_struct(&result_cf);

    ret |= cfg_check_config_file(RESULT_CONFIG_FILE);

    ret |= cfg_read_config_file(&result_cf,RESULT_CONFIG_FILE);

    return ret;
}

int store_result_flag(struct test_Parameters *test_para, int item)
{
    char result_buffer[5];

    if( item < 0 || item > test_para->total_num )
	return False;

    memset(result_buffer,0,5);
    sprintf(result_buffer,"%d",test_para->result_flag[item]);
    if ( cfg_add_key(&result_cf,test_para->test_order[item],result_buffer) < 0 )
	return False;

    if( cfg_write_config_file(&result_cf,RESULT_CONFIG_FILE) < 0 )
	return False;

    return True;
}

static int init_result_conf(struct test_Parameters *test_para)
{
    int i;
    char result_buffer[5];

    if( result_cf_init() < 0 ) { // if result config file init fail, made a default result.
	for(i = 0; i<test_para->total_num; i++) {
	    memset(result_buffer,0,5);
	    sprintf(result_buffer,"%d",UNTEST);
	    if ( cfg_add_key(&result_cf,test_para->test_order[i],result_buffer) < 0 ) {
		return False;
	    }
	}

	if( cfg_write_config_file(&result_cf,RESULT_CONFIG_FILE) < 0 )
	    return False;

	if( result_cf_init() < 0 )
	    return False;
    }

    return True;
}

int init_result_flag(struct test_Parameters *test_para)
{
    int i;

    if(init_result_conf(test_para) < 0)
    {
	return False;
    }

    for(i = 0; i < test_para->total_num; i++)
    {
	test_para->result_flag[i] =
	    cfg_get_key_value_to_int(result_cf,test_para->test_order[i]);
	debug_print("result_flag[%d] is %d\n",i,test_para->result_flag[i]);
    }

    return True;
}

void deinit_result_conf(void)
{
    cfg_free_config_file_struct(&result_cf);
}

