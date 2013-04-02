#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "key_test.h"
#include "debug.h"
#include "sdl_interface.h"
#include "init_parameters.h"
#include "sdl_key_def.h"

static SDL_Color Bcolor = {0,0,0};

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
#ifdef H350
    unsigned int keyval = 0,keyval1 = 0;
    int ret = 0;

    ret |= read(key_pad, &keyval, 4);
    usleep(10000);
    ret |= read(key_pad, &keyval1, 4);

    if(ret < 0)
	debug_print("read keypad error!\n");
    //debug_print("value is 0x%x :0x%x\n",keyval,keyval1);
    if(keyval == keyval1)
	return keyval;
    else
	return 0xffff;
#else
    SDLKey key;
    SDL_Event event;
    unsigned int keyval = 0;
    if (SDL_PollEvent(&event))
    {
	if(event.type == SDL_KEYDOWN)
	{
	    key = event.key.keysym.sym;

	    switch(key)
	    {
		case H350_SDL_BUTTON_UP:
		    keyval = (1 << H350_KEY_UP);
		    break;
		case H350_SDL_BUTTON_DOWN:
		    keyval = (1 << H350_KEY_DOWN);
		    break;
		case H350_SDL_BUTTON_B:
		    keyval = (1 << H350_KEY_B);
		    break;
		case H350_SDL_BUTTON_A:
		    keyval = (1 << H350_KEY_A);
		    break;
		default:break;
	    }
	}
    }

    return keyval;
#endif
}

void wait_for_next(void)
{
    unsigned int key_read_value = 0;
    unsigned int key_read_value1 = 0;

    while(key_read_value == 0)
    {
        key_read_value = key_pad_read();
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

static void key_release_warning(void)
{
    int key_read_value = 0;
    int key_loop = 1;

    test_words_show("Please release the key!",Bcolor);

#ifdef H350
    while(key_loop)
    {
	key_read_value = key_pad_read();
	if(key_read_value == 0xffff)
	    continue;

	if(key_read_value == 0)
	    key_loop = 0;
    }
#endif
}

int decision_loop(void)
{
    int decision_flag = 1;
    unsigned int key_read_value = 0;

    test_words_show("Press A for pass, B for fail",Bcolor);

    while(decision_flag)
    {
	key_read_value = key_pad_read();

	if(key_read_value == (1 << H350_KEY_A))
	{
	    draw_decision_pic(PASS);
	    return PASS;
	}
	else if(key_read_value == (1 << H350_KEY_B))
	{
	    draw_decision_pic(FAIL);
	    return FAIL;
	}
	else;
    }

    return FAIL;
}

int press_B_to_quit(void)
{
    int quit_loop = 1;
    unsigned int key_read_value = 0;

    test_words_show("Press A go on scaning, B jump off",Bcolor);

    while(quit_loop)
    {
	key_read_value = key_pad_read();

	if(key_read_value == (1 << H350_KEY_A))
	{
	    return PASS;
	}
	else if(key_read_value == (1 << H350_KEY_B))
	{
	    return FAIL;
	}
	else;
    }

    return FAIL;
}

int press_A_go_on(void)
{
    int go_on_loop = 1;
    unsigned int key_read_value = 0;

    while(go_on_loop)
    {
	key_read_value = key_pad_read();

	if(key_read_value == (1 << H350_KEY_A))
	{
	    return PASS;
	}
    }

    return FAIL;
}

int select_test_key_loop(struct test_Parameters *test_para)
{
    int loop = 1;
    unsigned int key_read_value = 0;

    while(loop)
    {
	key_read_value = key_pad_read();

	if(key_read_value == (1 << H350_KEY_DOWN))
	{
	    test_para->test_offset += 1;
	    if(test_para->test_offset > test_para->total_num - 1) 
	    {
		test_para->test_offset = 0;
	    }
	    test_para->select_mode = True;
	    return True;
	}

	if(key_read_value == (1 << H350_KEY_UP))
	{
	    test_para->test_offset -= 1;
	    if(test_para->test_offset < 0)
	    {
		test_para->test_offset = test_para->total_num - 1;
	    }
	    test_para->select_mode = True;
	    return True;
	}

	if(key_read_value == (1 << H350_KEY_A))
	{
	    loop = 0;
	    if(test_para->test_offset >= 0)
		test_para->select_mode = True;
	    return False;
	}

	if(key_read_value == (1 << H350_KEY_B))
	{
	    loop = 0;
	    test_para->select_mode = False;
	    return False;
	}
    }

    return False;
}
unsigned int direction_keys[]=
{
    (1 << H350_KEY_UP),
    (1 << H350_KEY_DOWN),
    (1 << H350_KEY_LEFT),
    (1 << H350_KEY_RIGHT),
    ((1 << H350_KEY_UP) | (1 << H350_KEY_LEFT)),
    ((1 << H350_KEY_DOWN) | (1 << H350_KEY_LEFT)),
    ((1 << H350_KEY_UP) | (1 << H350_KEY_RIGHT)),
    ((1 << H350_KEY_DOWN) | (1 << H350_KEY_RIGHT)),
};
/****************************key test****************************/
static int key_image_init(void)
{
    background = load_image(WHITE_BACKGROUND);

    return True;
}

static void key_image_deinit(void)
{
    sdl_free_surface(background);
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
    /*for(i=0; i < para->key_num; i++)*/
    /*{*/
	/*debug_print("x is %d, y is %d, w is %d, h is %d\n",para->key_rect[i].x,para->key_rect[i].y,para->key_rect[i].w,para->key_rect[i].h);*/
    /*}*/

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
    int i = 0;

    for(i = 0; i < para->key_num; i++)
    {
	if(keyval == para->key_value[i])
	{
	    ret = process_key_down(i,para);
	}
    }

    return ret;
}

int key_test(struct test_Parameters *test_para)
{
    int key_loop = 1;
    int key_read_value = 0;
    struct key_test_para key_para;
    init_flag(&key_para, test_para->key_rect_array, test_para->key_num);
    test_words_show("Keypad test!",Bcolor);
    key_release_warning();
    key_image_init();

    draw_key_view(&key_para);
#ifdef H350
    while(key_loop)
    {
	key_read_value = key_pad_read();

	if(!process_key(key_read_value,&key_para))
	    break;
    }
#else
    sleep(2);
#endif

    deinit_flag_res(&key_para);
    key_image_deinit();
    draw_decision_pic(PASS);

    return True;
}
//***************************joystick test****************
int joystick_test(struct test_Parameters *test_para)
{
    int key_loop = 1;
    int key_read_value = 0;
    struct key_test_para key_para;
    init_flag(&key_para, test_para->joykey_rect_array, test_para->joykey_num);

    key_image_init();
    test_words_show("Joystick test",Bcolor);
    key_release_warning();
    draw_key_view(&key_para);
#ifdef H350
    while(key_loop)
    {
	key_read_value = key_pad_read();

	if(!process_key(key_read_value,&key_para))
	    break;
    }
#else
    sleep(2);
#endif

    deinit_flag_res(&key_para);
    key_image_deinit();
    draw_decision_pic(PASS);

    return True;
}

//*********************gsensor test interface*****************
int gsensor_test_loop(struct test_Parameters *test_para)
{
    int gsensor_loop = 1;
    unsigned int key_read_value = 0;
    struct key_test_para key_para;

    init_flag(&key_para, test_para->gsensor_rect_array, test_para->gsensor_direction_num);
    key_image_init();
    draw_key_view(&key_para);
#ifdef H350
    while(gsensor_loop)
    {
	key_read_value = key_pad_read();

	if(!process_key(key_read_value,&key_para))
	    break;
    }
#else
    sleep(2);
#endif

    deinit_flag_res(&key_para);
    key_image_deinit();
    draw_decision_pic(PASS);

    return True;
}
