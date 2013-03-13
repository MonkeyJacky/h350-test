#include <fcntl.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "key_test.h"
#include "debug.h"
#include "sdl_shape.h"
#include "init_parameters.h"

static SDL_Color Bcolor={0,0,0};
static SDL_Surface* background = NULL;
enum KEY_ARRAY{
    H350_KEY_UP,
    H350_KEY_DOWN,
    H350_KEY_LEFT,
    H350_KEY_RIGHT,
    H350_KEY_A,
    H350_KEY_B,
    H350_KEY_Y,
    H350_KEY_X,
    H350_KEY_L,
    H350_KEY_R,
    H350_KEY_START,
    H350_KEY_SELECT,
};

struct key_test_para{
    int *key_flag;
    int key_count;
    int key_num;
    unsigned int *key_value;
    SDL_Rect *key_rect;
};

enum JOYSTICK_KEY{
    H350_JOYKEY_UP,
    H350_JOYKEY_DOWN,
    H350_JOYKEY_LEFT,
    H350_JOYKEY_RIGHT,
    H350_JOYKEY_TOP_LEFT,
    H350_JOYKEY_BOTTOM_LEFT,
    H350_JOYKEY_TOP_RIGHT,
    H350_JOYKEY_BOTTOM_RIGHT,
};
/************************key interfaces**************************/
static int key_pad = 0;
void init_key_pad(void)
{
    key_pad = open(KEYPAD, O_RDONLY);
    if(key_pad < 0)
    {
	key_pad = 0;
	debug_print("key pad open error!\n");
    }
}

void deinit_key_pad(void)
{
    if(key_pad)
    {
	close(key_pad);
	key_pad = 0;
    }
}

unsigned int key_pad_read(void)
{
    unsigned int keyval,keyval1;

    read(key_pad, &keyval, 4);
    usleep(10000);
    read(key_pad, &keyval1, 4);
    //debug_print("value is 0x%x :0x%x\n",keyval,keyval1);
    if(keyval == keyval1)
	return keyval;
    else
	return 0xffff;
}

void wait_for_next(void)
{
    unsigned int key_read_value = 0;
    unsigned int key_read_value1 = 0;

    while(key_read_value == 0)
    {
        key_read_value = key_pad_read();
        usleep(10*1000);
        key_read_value1 = key_pad_read();
        if(key_read_value == key_read_value1)
        {
            ;
        }
        else
        {
            key_read_value = 0;
        }
    }
    while(key_read_value != 0)
    {
        key_read_value = key_pad_read();
        usleep(10*1000);
        key_read_value1 = key_pad_read();
        if(key_read_value == key_read_value1)
        {
            ;
        }
        else
        {
            key_read_value = 1;
        }
    }
}
/****************************key test****************************/
static void key_image_init(void)
{
    background = IMG_Load(WHITE_BACKGROUND);
}

static void key_image_deinit(void)
{
    if(background)
	SDL_FreeSurface(background);
}

static void init_flag(struct key_test_para *para, const struct key_rect_para key_offset_array,int test_key_num)
{
    int i;

    para->key_num = test_key_num;
    para->key_flag = malloc(sizeof(int) * para->key_num);
    para->key_value = malloc(sizeof(unsigned int) * para->key_num);
    para->key_rect = malloc(sizeof(SDL_Rect) * para->key_num);

    for(i=0; i < para->key_num; i++)
	para->key_flag[i] = 0;

    para->key_count = 0;

    for(i=0; i < para->key_num; i++)
    {
	para->key_rect[i].x = key_offset_array.x[i];
	para->key_rect[i].y = key_offset_array.y[i];
	para->key_rect[i].w = key_offset_array.w[i];
	para->key_rect[i].h = key_offset_array.h[i];
	/*debug_print("x is %d, y is %d, w is %d, h is %d\n",para->key_rect[i].x,para->key_rect[i].y,para->key_rect[i].w,para->key_rect[i].h);*/
    }

    for(i=0; i < para->key_num; i++)
	para->key_value[i] = 1 << i;
}

static void deinit_flag_res(struct key_test_para *para)
{
    deep_free(para->key_flag);
    deep_free(para->key_value);
    deep_free(para->key_rect);
}

static void draw_key_view(struct key_test_para *para)
{
    int i;

    sdl_draw_a_pic(background,NULL,NULL);
    sdl_set_pen(&Bcolor, 1);
    for(i=0; i < para->key_num; i++)
    {
	debug_print("x is %d, y is %d, w is %d, h is %d\n",para->key_rect[i].x,para->key_rect[i].y,para->key_rect[i].w,para->key_rect[i].h);
    }

    for(i=0; i < para->key_num; i++)
    {
	sdl_draw_rect(&para->key_rect[i]);
    }

    sdl_flip_screen();
}

static int process_key_down(int offset, struct key_test_para *para)
{
    int goon = 1;
    if(para->key_flag[offset] == 0)
    {
	para->key_flag[offset] = 1;
	(para->key_count) ++;
	sdl_set_pen(&Bcolor, 1);
	sdl_draw_rect_solid(&(para->key_rect[offset]));
    }

    sdl_flip_screen();

    if( para->key_count == para->key_num)
	goon = 0;

    return goon;
}

static int process_key(unsigned int keyval, struct key_test_para *para)
{
    int ret = 1;

    if(keyval == para->key_value[H350_KEY_UP])
	ret = process_key_down(H350_KEY_UP,para);
    if(keyval == para->key_value[H350_KEY_DOWN])
	ret = process_key_down(H350_KEY_DOWN,para);
    if(keyval == para->key_value[H350_KEY_LEFT])
	ret = process_key_down(H350_KEY_LEFT,para);
    if(keyval == para->key_value[H350_KEY_RIGHT])
	ret = process_key_down(H350_KEY_RIGHT,para);
    if(keyval == para->key_value[H350_KEY_A])
	ret = process_key_down(H350_KEY_A,para);
    if(keyval == para->key_value[H350_KEY_B])
	ret = process_key_down(H350_KEY_B,para);
    if(keyval == para->key_value[H350_KEY_Y])
	ret = process_key_down(H350_KEY_Y,para);
    if(keyval == para->key_value[H350_KEY_X])
	ret = process_key_down(H350_KEY_X,para);
    if(keyval == para->key_value[H350_KEY_L])
	ret = process_key_down(H350_KEY_L,para);
    if(keyval == para->key_value[H350_KEY_R])
	ret = process_key_down(H350_KEY_R,para);
    if(keyval == para->key_value[H350_KEY_START])
	ret = process_key_down(H350_KEY_START,para);
    if(keyval == para->key_value[H350_KEY_SELECT])
	ret = process_key_down(H350_KEY_SELECT,para);

    return ret;
}

int key_test(struct test_Parameters *test_para)
{
    int key_loop = 0;
    int key_read_value = 0;
    struct key_test_para key_para;
    init_flag(&key_para, test_para->key_rect_array, test_para->key_num);
    /*test_back_show(key_test_str);*/
    /*key_release_warning(key_loop);*/
    key_image_init();

    draw_key_view(&key_para);
    while(key_loop)
    {
	key_read_value = key_pad_read();

	if(!process_key(key_read_value,&key_para))
	    break;
	usleep(150*1000);
    }

    deinit_flag_res(&key_para);
    key_image_deinit();

    return True;
}
//***************************joystick test****************

static int process_joystick_key(unsigned int keyval,struct key_test_para *para)
{
    int ret = 1;
    if(keyval == para->key_value[H350_JOYKEY_UP])
    {
	ret = process_key_down(H350_JOYKEY_UP,para);
    }
    if(keyval == para->key_value[H350_JOYKEY_DOWN])
    {
	ret = process_key_down(H350_JOYKEY_DOWN,para);
    }
    if(keyval == para->key_value[H350_JOYKEY_LEFT])
    {
	ret = process_key_down(H350_JOYKEY_LEFT,para);
    }
    if(keyval == para->key_value[H350_JOYKEY_RIGHT])
    {
	ret = process_key_down(H350_JOYKEY_RIGHT,para);
    }
    if(keyval == (para->key_value[H350_JOYKEY_UP] & para->key_value[H350_JOYKEY_LEFT]))
    {
	ret = process_key_down(H350_JOYKEY_TOP_LEFT,para);
    }
    if(keyval == (para->key_value[H350_JOYKEY_UP] & para->key_value[H350_JOYKEY_RIGHT]))
    {
	ret = process_key_down(H350_JOYKEY_TOP_RIGHT,para);
    }
    if(keyval == (para->key_value[H350_JOYKEY_DOWN] & para->key_value[H350_JOYKEY_RIGHT]))
    {
	ret = process_key_down(H350_JOYKEY_BOTTOM_RIGHT,para);
    }
    if(keyval == (para->key_value[H350_JOYKEY_DOWN] & para->key_value[H350_JOYKEY_LEFT]))
    {
	ret = process_key_down(H350_JOYKEY_BOTTOM_LEFT,para);
    }

    return ret;
}

int joystick_test(struct test_Parameters *test_para)
{
    int key_loop = 0;
    int key_read_value = 0;
    struct key_test_para key_para;
    init_flag(&key_para, test_para->joykey_rect_array, test_para->joykey_num);

    key_image_init();
    draw_key_view(&key_para);
    while(key_loop)
    {
	key_read_value = key_pad_read();

	if(!process_joystick_key(key_read_value,&key_para))
	    break;
	usleep(150*1000);
    }

    deinit_flag_res(&key_para);
    key_image_deinit();
    return True;
}
