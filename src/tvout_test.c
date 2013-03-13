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

static int get_hdmi_mode(void)
{
    int hdmi_set = 0;

    //TODO get hdmi mode 
    //return HDMI_SET_OFF or HDMI_SET_ON.

    return hdmi_set;
}

static int set_lcd_control_mode(int mode)
{
    int fb = -1;
    int ret = -1;
    int i = 50;
    char temp_command[MAX_SIZE] = {0};

    debug_print("mode is %d\n",mode);
    if(mode == PANEL_MODE_LCD)
    {
	sprintf(temp_command,"echo 0 > %s",HDMI_PROC_DEV);
	system(temp_command);
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
	close(fb);
	if(mode != PANEL_MODE_LCD)
	{
	    memset(temp_command,0,MAX_SIZE);
	    sprintf(temp_command,"echo 1 > %s",HDMI_IC_DEV);
	    system(temp_command);
	    while(i--)
		usleep(100*1000);
	}

	return True;
    }

    debug_print("fb set failed!\n");

    close(fb);

    return True;
}

static int test_hdmi_loop(void)
{
    if(get_hdmi_mode() == HDMI_SET_ON)
    {
	if(False == set_lcd_control_mode(DEFAULT_HDMI_MODE))
	    return False;
    }

    return True;
}

int hdmi_test(void)
{
    if (test_hdmi_loop() < 0)
	return False;

    return True;
}
