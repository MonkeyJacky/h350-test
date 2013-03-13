#ifndef _BAT_TEST_H_
#define _BAT_TEST_H_

#include "init_parameters.h"

#define BATTERY_DEVICE "/proc/jz/battery"
#define READ_BAT_TIMES 5

int battery_voltage_test(struct test_Parameters *test_para);

#endif
