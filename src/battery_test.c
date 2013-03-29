#include <stdio.h>
#include <unistd.h>
#include "battery_test.h"
#include "init_parameters.h"
#include "debug.h"
#include "sdl_shape.h"

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

int battery_voltage_test(struct test_Parameters *test_para)
{
    int bat_test_loop = 1;
    int count = 0;
    float voltage = 0;
    char temp_str[MAX_SIZE] = {0};

    test_words_show("Battery voltage test",Bcolor);
    if(init_battery_dev() < 0)
	return False;

    while(bat_test_loop)
    {
	voltage =  (float) get_bat_val();
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

    deinit_battery_dev();

    return True;
}
