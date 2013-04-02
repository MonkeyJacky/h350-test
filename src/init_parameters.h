#ifndef _PARAMETERS_HEAD_
#define _PARAMETERS_HEAD_

#include <string.h>

#define CONFIG_FILE "res/test.conf"
#define False (-1)
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
#define HDMI_SCREEN_WIDTH "hdmi_screen_width"
#define HDMI_SCREEN_HEIGHT "hdmi_screen_height"
#define deep_free(x)\
	free(x);\
	x = NULL;
#define WHITE_BACKGROUND "res/white_back.png"
#define PASS_VIEW "res/test_success.png"
#define FAIL_VIEW "res/test_fail.png"
#define FRAME_BUFFER_DEV "/dev/fb0"

#define FAIL (-1)
#define PASS 0
#define UNTEST 2
//*********************************************************
#define RESULT_CONFIG_FILE "res/result_file.conf"
#define JOYSTICK_RESULT "joystick_result"
#define KEYPAD_RESULT "keypad_result"
#define HEADPHONE_RESULT "headphone_result"
#define SPEAKER_RESULT "speaker_result"
#define LCD_RESULT "lcd_result"
#define TFCARD_RESULT "tfcard_result"
#define AVOUT_RESULT "avout_result"
#define HDMI_RESULT "hdmi_result"
#define WIFI_RESULT "wifi_result"
#define INTERNAL_CARD_RESULT "internal_card_result"
#define BAT_RESULT "bat_result"
//*********************************************************
#define RESULT_VIEW_NUM "result_view_size"
#define TEST_RESULT_WORD "test_result_word"
#define FAIL_COLOR "fail_color"
#define PASS_COLOR "pass_color"
#define UNTEST_COLOR "untest_color"

struct _result_view_para{
    int array_size;
    char** result_words;
    int* fail_color;
    int* pass_color;
    int* untest_color;
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
    int test_offset;
    int select_mode;
    int key_num;
    int joykey_num;
    char** test_order;
    int* result_flag;
    struct key_rect_para key_rect_array;
    struct key_rect_para joykey_rect_array;
    struct _screen_info_ screen_info;
    struct _screen_info_ hdmi_info;
    struct _result_view_para result_view_para;
};

int init_fb(int buffer_size);
void deinit_fb(int buffer_size);
int init_res(struct test_Parameters *test_para);
void deinit_res(struct test_Parameters *test_para);
int init_result_flag(struct test_Parameters *test_para);
void deinit_result_conf(void);
int store_result_flag(struct test_Parameters *test_para, int item);
#endif
