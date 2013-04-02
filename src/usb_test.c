#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "usb_test.h"
#include "sdl_interface.h"
#include "battery_test.h"
#include "debug.h"

static SDL_Color Bcolor = {0,0,0};
static int get_udc_state(void)
{
    FILE* udc_fd = 0;
    char buf[128];

    udc_fd = fopen(USB_DEVICES,"r");

    if (udc_fd == NULL){
	debug_print("can't open file : %s\n",USB_DEVICES);
	return OFFLINE;
    }else{
	rewind(udc_fd);
	memset(buf,0,128);
	fgets(buf,127,udc_fd);

	if(get_bat_dev_val() & 0x80000000)//The state of usb detecting feet.
	{
	    if (!strncasecmp(buf,"usb",3)){
		fclose(udc_fd);
		return USB;
	    }else if (!strncasecmp(buf,"power",5)){
		fclose(udc_fd);
		return POWER;
	    }else{
		fclose(udc_fd);
		return POWER;
	    }
	}
	else
	{
	    fclose(udc_fd);
	    return OFFLINE;
	}
    }

    return OFFLINE;
}

int udc_test(struct test_Parameters *test_para)
{
    int udc_loop = 1;
    int usb_detected = 0;
    int udc_state = OFFLINE;
    //Mount usb device.
    /*system("udc_connect.sh");*/
    test_words_show("Usb test",Bcolor);
#ifdef H350
    while(udc_loop){
	sleep(3);
	udc_state = get_udc_state();
	if (udc_state == USB)
	{
	    test_words_show("Detected usb insert",Bcolor);
	    sleep(1);
	    test_words_show("Please unpluging the usb cable",Bcolor);
	    usb_detected = 1;
	}
	else if (udc_state == OFFLINE)
	{
	    if(usb_detected)
	    {
		draw_decision_pic(PASS);
		return True;
	    }
	}
	else
	{
	    udc_loop = 0;
	}
    }
#endif
    //Umount usb device.
    /*system("udc_disconnect.sh");*/
    draw_decision_pic(FAIL);
    return False;
}
