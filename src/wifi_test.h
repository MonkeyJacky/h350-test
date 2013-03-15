#ifndef _WIFI_TEST_H_
#define _WIFI_TEST_H_

#define WIFI_DEVICE "/proc/jz/wifi_pw"
#define DRIVER_DIR "/usr/local/apps/p2p/8192cu.ko"
#define SUPPLICANT_CONFIG_FILE "/mnt/memory/wpa_supplicant.conf"
#define WPA_SUPPLICANT "wpa_supplicant"
#define NETWORK_CARD "wlan0"
#define HOST_IP "192.168.1.1"
#define CLIENT_IP_DEFAULT "192.168.1.2"
#define CONNECT_TIMES 10

struct Wifi_parameters{
    char *wifi_device;
    char *driver_dir;
    char *network_card;
    char *host_ip;
    char *client_ip;
    char *conf;
    char *wpa_supplicant;
};

int wifi_test();
#endif
