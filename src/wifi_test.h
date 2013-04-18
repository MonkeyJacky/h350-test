#ifndef _WIFI_TEST_H_
#define _WIFI_TEST_H_

#define WIFI_DEVICE "/proc/jz/wifi_pw"
#define DRIVER_DIR "res/wifi_modules.ko"
#ifdef H350
#define SUPPLICANT_CONFIG_FILE "/boot/res/wpa_supplicant.conf"
#else
#define SUPPLICANT_CONFIG_FILE "res/wpa_supplicant.conf"
#endif
#define WPA_SUPPLICANT "wpa_supplicant"
#define NETWORK_CARD "wlan0"
#define HOST_IP "192.168.0.1"
#define CLIENT_IP_DEFAULT "192.168.0.2"
#define CONNECT_TIMES 5
#define PRIORITY_SUPPLICANT_FILE "/mnt/mmc/wpa_supplicant.conf"

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
