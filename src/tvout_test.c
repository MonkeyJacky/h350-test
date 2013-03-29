#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "init_parameters.h"
#include "debug.h"
#include "tvout_test.h"
#include "sdl_shape.h"
#include "key_test.h"
#include "lcd_test.h"

static SDL_Color Bcolor = {0,0,0};
static SDL_Surface* test_back_show = NULL;
static int init_tvout_pic(void)
{
    test_back_show = load_image(TEST_BACK_IMG);
    if(test_back_show < 0)
	return False;

    return True;
}

static void deinit_tvout_pic(void)
{
    sdl_free_surface(test_back_show);
}

static int get_hdmi_mode(void)
{
    int hdmi_set = 0;
    test_words_show("Plug in HDMI cable, then press A go on",Bcolor);

    if(press_A_go_on() == PASS)
	hdmi_set = 1;

    return hdmi_set;
}

static int set_lcd_control_mode(int mode)
{
    int fb = -1;
    int ret = -1;
    char temp_command[MAX_SIZE] = {0};

    debug_print("mode is %d\n",mode);
    if(mode == PANEL_MODE_LCD)
    {
	sprintf(temp_command,"echo 0 > %s",HDMI_PROC_DEV);
	ret = system(temp_command);
	usleep(100*1000);
    }

    fb = open(FRAME_BUFFER_DEV,O_RDWR);
    if(fb < 0)
    {
	debug_print("open %s error",FRAME_BUFFER_DEV);
	return False;
    }

    ret = ioctl(fb,FBIO_MODE_SWITCH,mode);
    if(ret >= 0)
    {
	debug_print("fb set successed!\n");

	if(fb >= 0)
	{
	    close(fb);
	    fb = -1;
	}

	if(mode != PANEL_MODE_LCD)
	{
	    memset(temp_command,0,MAX_SIZE);
	    sprintf(temp_command,"echo 1 > %s",HDMI_IC_DEV);
	    ret = system(temp_command);
	    /*while(i--)*/
		/*usleep(100*1000);*/
	}

	return True;
    }

    debug_print("fb set failed!\n");

    if(fb >= 0)
    {
	close(fb);
    }

    return True;
}

static int test_hdmi_loop(struct test_Parameters *test_para)
{
    int i;

    extern unsigned char *fb;

    if(get_hdmi_mode() == HDMI_SET_ON)
    {
	if(False == set_lcd_control_mode(DEFAULT_HDMI_MODE))
	    return False;

	for(i = 0; i < 3; i++) // 3 kind of colors.
	{
	    draw_color_bar((unsigned int *)fb,i,test_para->hdmi_info.width,test_para->hdmi_info.height);
	    sleep(2);
	}

	set_lcd_control_mode(PANEL_MODE_LCD);
    }

    return True;
}

int hdmi_test(struct test_Parameters *test_para)
{
    test_words_show("HDMI test",Bcolor);

    if(init_tvout_pic() < 0)
    {
	draw_decision_pic(FAIL);
	return False;
    }

    if(init_fb(test_para->hdmi_info.buffer_size) < 0)
    {
	draw_decision_pic(FAIL);
	return False;
    }

    if (test_hdmi_loop(test_para) != True)
    {
	draw_decision_pic(FAIL);
	return False;
    }

    deinit_fb(test_para->hdmi_info.buffer_size);
    deinit_tvout_pic();
    return decision_loop();
}

//**************************avout test*********************
static FILE* av_fp=NULL;
static int init_av_dev()
{
    av_fp = fopen(AVOUT_DEV,"rb");
    if(!av_fp)
    {
	debug_print("open avout dev error!\n");
	return False;
    }
    
    return True;
}

static void deinit_av_dev()
{
    if(av_fp)
    {
	fclose(av_fp);
	av_fp = NULL;
    }
}

static int get_av_out_mode()
{
    int avout_mode = 0;
    char buf[128] = {0};
    if(av_fp)
    {
	rewind(av_fp);
	if ( !fgets(buf, 127, av_fp) )
	    return 0;
	sscanf(buf, "%d", &avout_mode);
    }

    return avout_mode;
}

int avout_test(struct test_Parameters *test_para)
{
    int avout_loop = 1;

    test_words_show("Av out test",Bcolor);

#ifdef H350
    if(init_tvout_pic() < 0)
    {
	draw_decision_pic(FAIL);
	return False;
    }

    if(init_av_dev() < 0)
    {
	draw_decision_pic(FAIL);
	return False;
    }

    while(avout_loop)
    {
	if(get_av_out_mode() == 1)
	{
	    sdl_draw_a_pic(test_back_show,NULL,NULL);
	    sleep(3);
	}

	if(press_B_to_quit() < 0)
	{
	    draw_decision_pic(FAIL);
	    return False;
	}

	usleep(200*1000);
    }

    deinit_av_dev();
    deinit_tvout_pic();
#endif

    return decision_loop();
}
