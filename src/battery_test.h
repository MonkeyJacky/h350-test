#ifndef _BAT_TEST_H_
#define _BAT_TEST_H_

#include "init_parameters.h"

#define BATTERY_DEVICE "/proc/jz/battery"
#ifdef H350
#define READ_BAT_TIMES 5
#else
#define READ_BAT_TIMES 1
#endif

int battery_voltage_test(struct test_Parameters *test_para);

#endif
