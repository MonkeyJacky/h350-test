#include <stdio.h>
#include <unistd.h>
#include "battery_test.h"
#include "init_parameters.h"
#include "debug.h"
#include "sdl_interface.h"

static SDL_Color Bcolor = {0,0,0};
static FILE* fp = NULL;
static int init_battery_dev(void)
{
    fp = fopen(BATTERY_DEVICE,"rb");
    if (!fp){
	debug_print("can not open /proc/jz/battery!\n");
	return False;
    }

    return True;
}

static void deinit_battery_dev(void)
{
    if(fp)
	fclose(fp);
}

static int get_bat_val(void)
{
    char buf[128] = {0};
    int mvolts = 0;

    if(fp)
    {
	rewind(fp);
	if(!fgets(buf,127,fp))
	    return 0;
	sscanf(buf,"%d",&mvolts);
    }

    return (mvolts&0x0000ffff);
}

int get_bat_dev_val(void)
{
    char buf[128] = {0};
    int mvolts = 0;
    FILE* bat_fp = NULL;

    bat_fp = fopen(BATTERY_DEVICE,"rb");
    if(!bat_fp){
	debug_print("can not open %s\n",BATTERY_DEVICE);
	return False;
    }

    rewind(bat_fp);
    fgets(buf,127,bat_fp);
    sscanf(buf,"%d",&mvolts);

    if(bat_fp)
	fclose(bat_fp);

    return mvolts;
}

int battery_voltage_test(struct test_Parameters *test_para)
{
    int bat_test_loop = 1;
    int count = 0;
    float voltage = 0;
    char temp_str[MAX_SIZE] = {0};

    test_words_show("Battery voltage test",Bcolor);
#ifdef H350
    if(init_battery_dev() < 0)
	return False;
#endif

    while(bat_test_loop)
    {
#ifdef H350
	voltage =  (float) get_bat_val();
#endif
	sleep(1);
	memset(temp_str,0,MAX_SIZE);
	sprintf(temp_str,"Voltage : %0.2f V",voltage/1000);
	test_words_show(temp_str,Bcolor);
	count ++;
	if(count == READ_BAT_TIMES)
	{
	    bat_test_loop = 0;
	}
    }

#ifdef H350
    deinit_battery_dev();
#endif

    return True;
}
