#ifndef _USB_TEST_H_
#define _USB_TEST_H_

#include "init_parameters.h"

#define USB_DEVICES "/sys/devices/platform/musb_hdrc.0/uh_cable"
enum UDC_MODE{
    OFFLINE,USB,POWER,
};

int udc_test(struct test_Parameters *test_para);

#endif
