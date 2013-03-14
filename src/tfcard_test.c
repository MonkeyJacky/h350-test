#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "init_parameters.h"
#include "tfcard_test.h"
#include "debug.h"
#include "sdl_shape.h"

static SDL_Color Bcolor = {0,0,0};
static FILE* tfcard_fp = NULL;
static int tfcard_scan_init()
{
    if(!tfcard_fp)
    {
	tfcard_fp = fopen(TFCARD_DEVICE,"rb");
	if(!tfcard_fp)
	{
	    tfcard_fp = NULL;
	    return False;
	}
    }
    return True;
}

static int get_tfcard_state()
{
    char tfcard_state[20] = {0};
    int temp_draw_tf_card = -1;
    if(tfcard_fp)
    {
	rewind(tfcard_fp);
	fgets(tfcard_state,20,tfcard_fp);
	if(strstr(tfcard_state,"no"))
	{
	    temp_draw_tf_card = False;
	}
	else
	{
	    temp_draw_tf_card = True;
	}
    }
    PRINT_VALUE(temp_draw_tf_card,%d);

    return temp_draw_tf_card;
}

void deinit_tfcard(void)
{
    if(tfcard_fp)
	fclose(tfcard_fp);
}

static int check_tfcard_info(void)
{
    int i;
    int ok_count = 0;

    for(i = 0; i < CHECK_TIMES; i++)
    {
	if(access(NOTICE_FILE,W_OK) == 0 && access(NOTICE_FILE,R_OK) == 0)
	{
	    ok_count ++;
	}
	else
	{
	    ok_count = 0;
	}
    }

    if(ok_count >= ENOUGH_TIMES)
	return True;
    else
	return False;
}

int tfcard_test(struct test_Parameters *test_para)
{
    int once = 1;
    int tfcard_loop = 1;
    int tfcard_flag = 1;

    test_words_show("External card test",Bcolor);
    while(tfcard_loop)
    {
	if(once){
	    test_words_show("Please insert the SD card",Bcolor);
	}
	//The SD card must have one file named test_mode.txt
	//for card checking.
	if(tfcard_scan_init() == True)
	{
	    usleep(500*1000);
	    if(get_tfcard_state() == True)
	    {
		if(once)
		{
		    sleep(1);
		    tfcard_flag = check_tfcard_info();
		    once = 0;
		    if(tfcard_flag == False)
		    {
			debug_print("Tfcard test failed!\n");
			deinit_tfcard();
			draw_decision_pic(FAIL);
			return False;
		    }
		    else
		    {
			test_words_show("Pull out the card to go on",Bcolor);
		    }
		}
	    }
	    else
	    {
		if(tfcard_flag == True && !once)
		{
		    debug_print("Passed tfcard test!\n");
		    deinit_tfcard();
		    draw_decision_pic(PASS);
		    return True;
		}
	    }
	}

	usleep(500*1000);
    }

    draw_decision_pic(FAIL);
    return False;
}
