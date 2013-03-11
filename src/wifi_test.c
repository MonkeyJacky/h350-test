//*************************************wifi test**********************************
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "debug.h"
#include "wifi_test.h"
#include "init_parameters.h"

static int allocate_ip_random(struct Wifi_parameters *wifi_para)
{
    struct timeval tpstart;
    unsigned int rand_data = 0;
    char temp_ip[MAX_SIZE] = {0};
    int ret = 0;
    ret = gettimeofday(&tpstart,NULL);
    if(ret < 0)
    {
	return False;
    }
    debug_print("tpstart.tv_usec is %d\n",(int)tpstart.tv_usec);
    srand((int)tpstart.tv_usec);

    do {
	rand_data = (unsigned int)(255 * ((float)rand() / (RAND_MAX + 1.0)));
	usleep(100*1000);
    } while (rand_data < 2);

    debug_print("rand_data is %d\n",rand_data);

    sprintf(temp_ip, "192.168.1.%d", rand_data);
    wifi_para->client_ip = temp_ip;

    return True;
}

static void init_wifi_para(struct Wifi_parameters *wifi_para)
{
    wifi_para->wifi_device = WIFI_DEVICE;
    wifi_para->driver_dir = DRIVER_DIR;
    wifi_para->network_card = NETWORK_CARD;
    wifi_para->host_ip = HOST_IP;
    if ( allocate_ip_random(wifi_para) < 0 )
    {
	wifi_para->client_ip = CLIENT_IP_DEFAULT;
    }
}

static int init_wifi_driver(void)
{
    char temp_command[MAX_SIZE];
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"echo 1 > %s",WIFI_DEVICE);
    system(temp_command);
    usleep(500*1000);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"insmod %s",DRIVER_DIR);
    system(temp_command);
    sleep(1);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"ifconfig %s up",NETWORK_CARD);
    system(temp_command);
    sleep(1);

    return True;
}

static void deinit_wifi_driver(void)
{
    char temp_command[MAX_SIZE];
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"ifconfig %s down",NETWORK_CARD);
    system(temp_command);
    usleep(500*1000);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"rmmode %s",DRIVER_DIR);
    system(temp_command);
    usleep(500*1000);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"echo 0 > %s",WIFI_DEVICE);
    system(temp_command);
    usleep(100*1000);
}

int wifi_test()
{
    struct Wifi_parameters wifi_para;
    init_wifi_para(&wifi_para);

    debug_print("wifi device is %s\n",wifi_para.wifi_device);
    debug_print("driver dir is %s\n",wifi_para.driver_dir);
    debug_print("network card is %s\n",wifi_para.network_card);
    debug_print("host ip is %s\n",wifi_para.host_ip);
    debug_print("client ip is %s\n",wifi_para.client_ip);

    return True;
}

