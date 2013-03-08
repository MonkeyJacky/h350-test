#ifndef _PARAMETERS_HEAD_
#define _PARAMETERS_HEAD_

#define WHITE_BACKGROUND "res/white_back.png"
#define CONFIG_FILE "test.conf"
#define False -1
#define True 0
#define CONTINUE 1
#define TEST_ORDER "test_order"
#define MAX_SIZE 256
#define TOTAL_NUM "test_total_num"
#define KEY_NUM "key_num"
#define RECT_ARRAY_X "key_rect_x"
#define RECT_ARRAY_Y "key_rect_y"
#define RECT_ARRAY_W "key_rect_w"
#define RECT_ARRAY_H "key_rect_h"
#define JOY_KEY_NUM "joykey_num"
#define JOYRECT_ARRAY_X "joykey_rect_x"
#define JOYRECT_ARRAY_Y "joykey_rect_y"
#define JOYRECT_ARRAY_W "joykey_rect_w"
#define JOYRECT_ARRAY_H "joykey_rect_h"
#define KEYPAD "/dev/keypad"
#define deep_free(x)\
	free(x);\
	x = NULL;
#define DEBUG
#ifdef DEBUG
#define debug_print(x,...)\
	printf(x,##__VA_ARGS__);
#define debug_print_s(x,...)\
        printf("%s() :%d "x"\n",__func__,__LINE__,##__VA_ARGS__);
#else
#define debug_print_s(x,...)
#define debug_print(x,...) 
#endif

#endif
