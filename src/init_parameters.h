#ifndef _PARAMETERS_HEAD_
#define _PARAMETERS_HEAD_

#include <string.h>

#define CONFIG_FILE "test.conf"
#define False -1
#define True 0
#define CONTINUE 1
#define MAX_SIZE 256
#define TEST_ORDER "test_order"
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
#define SCREEN_WIDTH "screen_width"
#define SCREEN_HEIGHT "screen_height"
#define SCREEN_BITS_PER_PIXEL "screen_bits_per_pixel"
#define deep_free(x)\
	free(x);\
	x = NULL;
#define WHITE_BACKGROUND "res/white_back.png"
#define PASS_VIEW "res/test_success.png"
#define FAIL_VIEW "res/test_fail.png"
#define FRAME_BUFFER_DEV "/dev/fb0"

enum SELECTION{
    FAIL,
    PASS,
};

struct key_rect_para{
    int	*x;
    int	*y;
    int	*w;
    int	*h;
};

struct _screen_info_{
    int width;
    int height;
    int bits_per_pixel;
    int buffer_size;
};

struct test_Parameters{
    int total_num;
    int key_num;
    int joykey_num;
    char** test_order;
    int* result_flag;
    struct key_rect_para key_rect_array;
    struct key_rect_para joykey_rect_array;
    struct _screen_info_ screen_info;
};

int init_fb(int buffer_size);
void deinit_fb(int buffer_size);
int init_res(struct test_Parameters *test_para);
void deinit_res(struct test_Parameters *test_para);
int init_result_flag(struct test_Parameters *test_para);
#endif
