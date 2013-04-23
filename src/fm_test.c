#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "fm_test.h"
#include "debug.h"
#include "sdl_interface.h"
#include "sound_test.h"

static SDL_Color Bcolor = {0,0,0};
int fm_test(struct test_Parameters *test_para)
{
    int freq = MIN_FREQUENCY;
    int fm_flag = False;
    int fm_loop = 1;

    test_words_show("FM test",Bcolor);
    test_words_show("Please plug in headphone...",Bcolor);
#ifdef H350
    set_volume(100);
    if(init_headphone()==False)
    {
	debug_print("init hp error!\n");
	return False;
    }
    enable_fm(1);

    while(fm_loop)
    {
	usleep(20*1000);

	if(hp_detect() == 1)
	{
	    int fm_fd = open(FM_DEVICE,O_RDWR,0);
	    if( fm_fd < 0 ){
		debug_print("open %s error!\n",FM_DEVICE);
		return False;
	    };

	    ioctl(fm_fd,FM_POWER_ON,NULL);

	    test_words_show("Searching....",Bcolor);
	    for(freq = MIN_FREQUENCY; freq < MAX_FREQUENCY; freq++)
	    {
		if(ioctl(fm_fd,FM_AUTO_SEEK,&freq))
		{
		    fm_flag = True;
		    test_words_show("Searched successfully",Bcolor);
		    fm_loop = 0;
		    break;
		}
	    }

	    if(fm_flag == True)
	    {
		ioctl(fm_fd,FM_SET_FREQ,&freq);
		sleep(5);
	    }

	    ioctl(fm_fd,FM_POWER_OFF,NULL);

	    close(fm_fd);
	    while(hp_detect() != 0)
	    {
		test_words_show("Please plug out the headphone...",Bcolor);
	    }
	}
    }

    enable_fm(0);
    deinit_headphone();
#endif
    return fm_flag;
}
