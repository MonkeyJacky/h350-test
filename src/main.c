#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <SDL.h>
#include "wejpconfig.h"
#include "sdl_shape.h"
#include "parameters.h"

SDL_Surface* background = NULL;
SDL_Surface* screen = NULL;
SDL_Color Bcolor = {0,0,0};
static ConfigFile test_cf;
struct key_rect_para{
    int	*x;
    int	*y;
    int	*w;
    int	*h;
};

struct test_Parameters{
    int total_num;
    int key_num;
    int joykey_num;
    char** test_order;
    int* result_flag;
    struct key_rect_para key_rect_array;
    struct key_rect_para joykey_rect_array;
};

void sdl_flip_screen(SDL_Surface* tmp_Screen)
{
    SDL_Flip(tmp_Screen);
}

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

    test_para->key_num = cfg_get_key_value_to_int(test_cf,KEY_NUM);
    test_para->key_rect_array.x = malloc(sizeof(int) * test_para->key_num);
    test_para->key_rect_array.y = malloc(sizeof(int) * test_para->key_num);
    test_para->key_rect_array.w = malloc(sizeof(int) * test_para->key_num);
    test_para->key_rect_array.h = malloc(sizeof(int) * test_para->key_num);

    test_para->joykey_num = cfg_get_key_value_to_int(test_cf,JOY_KEY_NUM);
    test_para->joykey_rect_array.x = malloc(sizeof(int) * test_para->joykey_num);
    test_para->joykey_rect_array.y = malloc(sizeof(int) * test_para->joykey_num);
    test_para->joykey_rect_array.w = malloc(sizeof(int) * test_para->joykey_num);
    test_para->joykey_rect_array.h = malloc(sizeof(int) * test_para->joykey_num);

    for(i = 0; i < test_para->total_num; i++)
    {
	test_para->test_order[i] = malloc(sizeof(char) * MAX_SIZE);
    }

    get_array_from_conf_str(test_para->test_order,TEST_ORDER,test_cf);
    get_array_from_conf_int(test_para->key_rect_array.x,RECT_ARRAY_X,test_cf);
    get_array_from_conf_int(test_para->key_rect_array.y,RECT_ARRAY_Y,test_cf);
    get_array_from_conf_int(test_para->key_rect_array.w,RECT_ARRAY_W,test_cf);
    get_array_from_conf_int(test_para->key_rect_array.h,RECT_ARRAY_H,test_cf);
    get_array_from_conf_int(test_para->joykey_rect_array.x,JOYRECT_ARRAY_X,test_cf);
    get_array_from_conf_int(test_para->joykey_rect_array.y,JOYRECT_ARRAY_Y,test_cf);
    get_array_from_conf_int(test_para->joykey_rect_array.w,JOYRECT_ARRAY_W,test_cf);
    get_array_from_conf_int(test_para->joykey_rect_array.h,JOYRECT_ARRAY_H,test_cf);

    for(i = 0; i < test_para->total_num; i++)
    {
	debug_print("test order[%d] is %s\n",i,test_para->test_order[i]);
    }

    /*for(i = 0; i < test_para->key_num; i++)*/
    /*{*/
	/*debug_print("key x is %d, key y is %d, key w is %d, key h is %d\n",test_para->key_rect_array.x[i],test_para->key_rect_array.y[i],test_para->key_rect_array.w[i],test_para->key_rect_array.h[i]);*/
    /*}*/

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

int init_sdl(void)
{
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
	debug_print("SDL_Init(SDL_INIT_VIDEO) FAIL!\n");
	return -1;
    }

    SDL_ShowCursor(0);

    screen = SDL_SetVideoMode( 320, 240, 32, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
	debug_print("SDL_SetVideoMode FAIL!\n");
	return -1;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
	debug_print("TTF_Init FAIL!\n");
	//        return -1;
    }

    /*font = TTF_OpenFont(FONT_PATH,18);*/
    background = IMG_Load(WHITE_BACKGROUND);
    /*lcd_test_pic = IMG_Load(LCD_TEST_PIC);*/
    /*TTF_SetFontStyle(font,TTF_STYLE_BOLD);*/
    return True;
}
/************************key interfaces**************************/
int key_pad = 0;
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

/****************************key test****************************/
enum{
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

    SDL_BlitSurface(background, NULL,screen, NULL);
    sdl_set_pen(&Bcolor, 1, screen);
    for(i=0; i < para->key_num; i++)
    {
	debug_print("x is %d, y is %d, w is %d, h is %d\n",para->key_rect[i].x,para->key_rect[i].y,para->key_rect[i].w,para->key_rect[i].h);
    }

    for(i=0; i < para->key_num; i++)
    {
	sdl_draw_rect(screen, &para->key_rect[i]);
    }

    sdl_flip_screen(screen);
}

static int process_key_down(int offset, struct key_test_para *para)
{
    int goon = 1;
    if(para->key_flag[offset] == 0)
    {
	para->key_flag[offset] = 1;
	(para->key_count) ++;
	sdl_set_pen(&Bcolor, 1, screen);
	sdl_draw_rect_solid(screen, &(para->key_rect[offset]));
    }

    sdl_flip_screen(screen);

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

static int key_test(struct test_Parameters *test_para)
{
    int key_loop = 0;
    int key_read_value = 0;
    struct key_test_para key_para;
    init_flag(&key_para, test_para->key_rect_array, test_para->key_num);
    /*test_back_show(key_test_str);*/
    /*key_release_warning(key_loop);*/

    draw_key_view(&key_para);
    while(key_loop)
    {
	key_read_value = key_pad_read();

	if(!process_key(key_read_value,&key_para))
	    break;
	usleep(150*1000);
    }

    deinit_flag_res(&key_para);

    return True;
}
//***************************joystick test****************
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

static int joystick_test(struct test_Parameters *test_para)
{
    int key_loop = 0;
    int key_read_value = 0;
    struct key_test_para key_para;
    init_flag(&key_para, test_para->joykey_rect_array, test_para->joykey_num);

    draw_key_view(&key_para);
    while(key_loop)
    {
	key_read_value = key_pad_read();

	if(!process_joystick_key(key_read_value,&key_para))
	    break;
	usleep(150*1000);
    }

    deinit_flag_res(&key_para);
    return True;
}

void test_loop(struct test_Parameters *test_para,int num)
{
    if(!strcmp(test_para->test_order[num],"lcd"))
    {
	debug_print("This is lcd test!\n");
//	lcd_test();
//	system("echo 50 > /proc/jz/lcd_backlight");
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
    }
    else if(!strcmp(test_para->test_order[num],"tfcard"))
    {
	debug_print("This is tfcard test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"internal_card"))
    {
	debug_print("This is internal_card test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"hdmi"))
    {
	debug_print("This is	hdmi test!\n");
    }
    else if(!strcmp(test_para->test_order[num],"tvout"))
    {
	debug_print("This is tvout test!\n");
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

    return 0;
}
