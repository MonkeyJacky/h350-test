#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "init_parameters.h"
#include "tfcard_test.h"

static FILE* tfcard_fp = NULL;
static int tfcard_scan_init()
{
    if(!tfcard_fp)
    {
	tfcard_fp = fopen(TFCARD_DEVICE,"rb");
	if(!tfcard_fp)
	    return False;
    }
    return True;
}

static int get_tfcard_state()
{
    char tfcard_state[20];
    int temp_draw_tf_card = 0;
    if(tfcard_fp)
    {
	rewind(tfcard_fp);
	fgets(tfcard_state,20,tfcard_fp);
	if(strstr(tfcard_state,"no"))
	{
	    temp_draw_tf_card = 0;
	    fclose(tfcard_fp);
	    tfcard_fp = NULL;
	}
	else
	{
	    temp_draw_tf_card = 1;
	}
    }

    return temp_draw_tf_card;
}

static int check_tfcard_info()
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

int tfcard_test(void)
{
    int once = 1;
    int tfcard_loop = 1;
    int tfcard_flag = 1;

    while(tfcard_loop)
    {
	if(tfcard_scan_init())
	{
	    usleep(500*1000);
	    if(get_tfcard_state() == 1)
	    {
		if(once)
		{
		    sleep(1);
		    tfcard_flag = check_tfcard_info();
		    once = 0;
		    tfcard_loop = tfcard_flag;
		    if(tfcard_flag == 0)
		    {
			//return fail warning show. 
		    }
		    else
		    {
			//show pull out
		    }
		}
	    }
	    else
	    {
		if(tfcard_flag && !once)
		{
		    //return ok warning show.
		}
	    }
	}

	usleep(500*1000);
    }

    return False;
}
