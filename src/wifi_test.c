//*************************************wifi test**********************************
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "debug.h"
#include "wifi_test.h"
#include "init_parameters.h"
#include "sdl_interface.h"

static SDL_Color Bcolor = {0,0,0};
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

    sprintf(temp_ip, "192.168.0.%d", rand_data);
    wifi_para->client_ip = malloc(MAX_SIZE);
    if(!wifi_para->client_ip)
    {
	debug_print("client ip malloc error!\n");
	return False;
    }
    memcpy(wifi_para->client_ip,temp_ip,MAX_SIZE);

    return True;
}

static void init_wifi_para(struct Wifi_parameters *wifi_para)
{
    wifi_para->wpa_supplicant = WPA_SUPPLICANT;
    wifi_para->driver_dir = DRIVER_DIR;
    wifi_para->network_card = NETWORK_CARD;
    wifi_para->host_ip = HOST_IP;
    if ( allocate_ip_random(wifi_para) < 0 )
    {
	wifi_para->client_ip = CLIENT_IP_DEFAULT;
    }

    if(access(PRIORITY_SUPPLICANT_FILE, F_OK) == 0)
	wifi_para->conf = PRIORITY_SUPPLICANT_FILE;
    else
	wifi_para->conf = SUPPLICANT_CONFIG_FILE;
}

static int init_wifi_driver(struct Wifi_parameters *wifi_para)
{
    char temp_command[MAX_SIZE];
    int ret = True;

    test_words_show("Loading wifi module...",Bcolor);
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"insmod %s",wifi_para->driver_dir);
    ret |= system(temp_command);
    sleep(2);
    if(ret < 0)
	return False;

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"ifconfig %s up",wifi_para->network_card);
    ret |= system(temp_command);
    sleep(2);
    if(ret < 0)
	return False;

    return True;
}

static void deinit_wifi_driver(struct Wifi_parameters *wifi_para)
{
    char temp_command[MAX_SIZE];
    int ret = 0;

    deep_free(wifi_para->client_ip);

    test_words_show("Unloading wifi module...",Bcolor);
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"killall -9 %s",wifi_para->wpa_supplicant);
    ret |= system(temp_command);
    usleep(100*1000);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"ifconfig %s down",wifi_para->network_card);
    ret |= system(temp_command);
    usleep(500*1000);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"rmmod %s",wifi_para->driver_dir);
    ret |= system(temp_command);
    usleep(500*1000);

    if(ret < 0)
	debug_print("deinit wifi driver error");
}

static int connection_loop(struct Wifi_parameters *wifi_para)
{
    int ret = -1;
    char temp_command[MAX_SIZE] = {0};

    test_words_show("Searching wifi hotspots...",Bcolor);
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"iwlist %s scanning > %s",wifi_para->network_card,ROUTES_LIST_FILE);
    ret = system(temp_command);
    sleep(5);

    FILE* fp = NULL;
    fp = fopen(ROUTES_LIST_FILE,"r");
    if(!fp)
    {
	return False;
    }

    char cha = 0;
    char *routesFileContents = NULL;
    int i = 0;
    int length = 0;

    fseek(fp,0,SEEK_END);
    length = ftell(fp);
    routesFileContents = malloc(length);
    if (!routesFileContents)
    {
	fclose(fp);
	return False;
    }
    memset(routesFileContents, 0, length);

    rewind(fp);
    while(1)
    {
	while(!feof(fp))
	{
	    cha = fgetc(fp);
	    routesFileContents[i++] = cha;
	}

	if(!strstr(routesFileContents,ROUTES_LABEL))
	{
	    ret = False;
	    break;
	}
	else
	{
	    ret = True;
	    break;
	}
    }

    fclose(fp);
    deep_free(routesFileContents);
    remove(ROUTES_LIST_FILE);

#if 0
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"%s -Dwext -i%s -c %s -dd &",wifi_para->wpa_supplicant,wifi_para->network_card,wifi_para->conf);
    ret = system(temp_command);
    sleep(5);

    while(1)
    {
	memset(temp_command,0,MAX_SIZE);
	sprintf(temp_command,"ifconfig %s %s",wifi_para->network_card,wifi_para->client_ip);
	ret = system(temp_command);
	if(ret == 0)
	    break;
	sleep(2);
    }

    test_words_show("Try to connect...",Bcolor);
    int count = 0;
    while(count < CONNECT_TIMES)
    {
	memset(temp_command,0,MAX_SIZE);
	sprintf(temp_command,"ping %s -c 5",wifi_para->host_ip);
	ret = system(temp_command);
	count ++;
    }
#endif

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

    debug_print("driver dir is %s\n",wifi_para.driver_dir);
    debug_print("network card is %s\n",wifi_para.network_card);
    debug_print("host ip is %s\n",wifi_para.host_ip);
    debug_print("client ip is %s\n",wifi_para.client_ip);
    test_words_show("Wifi test",Bcolor);
#ifdef H350
    ret |= init_wifi_driver(&wifi_para);

    ret |= connection_loop(&wifi_para);

    deinit_wifi_driver(&wifi_para);
#endif
    if(!ret)
    {
	draw_decision_pic(PASS);
	debug_print("It's connected successfully!\n");
    }
    else
    {
	draw_decision_pic(FAIL);
	debug_print("connected fail!\n");
	return False;
    }

    return True;
}

