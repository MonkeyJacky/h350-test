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
    wifi_para->conf = SUPPLICANT_CONFIG_FILE;
}

static int init_wifi_driver(struct Wifi_parameters *wifi_para)
{
    char temp_command[MAX_SIZE];
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"echo 1 > %s",wifi_para->wifi_device);
    system(temp_command);
    usleep(500*1000);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"insmod %s",wifi_para->driver_dir);
    system(temp_command);
    sleep(1);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"ifconfig %s up",wifi_para->network_card);
    system(temp_command);
    sleep(1);

    return True;
}

static void deinit_wifi_driver(struct Wifi_parameters *wifi_para)
{
    char temp_command[MAX_SIZE];
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"ifconfig %s down",wifi_para->network_card);
    system(temp_command);
    usleep(500*1000);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"rmmode %s",wifi_para->driver_dir);
    system(temp_command);
    usleep(500*1000);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"echo 0 > %s",wifi_para->wifi_device);
    system(temp_command);
    usleep(100*1000);
}

static int connection_loop(struct Wifi_parameters *wifi_para)
{
    int ret = 0;
    char temp_command[MAX_SIZE];

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"ifconfig %s %s",wifi_para->network_card,wifi_para->client_ip);
    ret |= system(temp_command);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"iwlist %s scanning",wifi_para->network_card);
    ret |= system(temp_command);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"wpa_supplicant -Dwext -i%s -c %s -dd &",wifi_para->network_card,wifi_para->conf);
    ret |= system(temp_command);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"ping %s -c 5",wifi_para->host_ip);
    ret |= system(temp_command);
    if(!ret)
	return True;
    else
	return False;
}

int wifi_test(struct test_Parameters *test_para)
{
    struct Wifi_parameters wifi_para;
    int ret = 0;
    init_wifi_para(&wifi_para);

    debug_print("wifi device is %s\n",wifi_para.wifi_device);
    debug_print("driver dir is %s\n",wifi_para.driver_dir);
    debug_print("network card is %s\n",wifi_para.network_card);
    debug_print("host ip is %s\n",wifi_para.host_ip);
    debug_print("client ip is %s\n",wifi_para.client_ip);
    init_wifi_driver(&wifi_para);

    ret = connection_loop(&wifi_para);
    if(!ret)
    {
	debug_print("It's connected successfully!\n");
    }
    else
    {
	debug_print("connected fail!\n");
	return False;
    }

    deinit_wifi_driver(&wifi_para);

    return True;
}

