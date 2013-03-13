#include <stdio.h>
#include <unistd.h>
#include "battery_test.h"
#include "init_parameters.h"
#include "debug.h"

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
	fgets(buf,127,fp);
	sscanf(buf,"%d",&mvolts);
    }

    return (mvolts&0x0000ffff);
}

int battery_voltage_test(void)
{
    int bat_test_loop = 1;
    int count = 0;

    if(init_battery_dev() < 0)
	return False;

    while(bat_test_loop)
    {
	get_bat_val();
	sleep(1);
	count ++;
	if(count == READ_BAT_TIMES)
	{
	    bat_test_loop = 0;
	}
    }

    deinit_battery_dev();

    return True;
}
