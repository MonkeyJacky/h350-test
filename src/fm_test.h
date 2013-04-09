#ifndef _FM_TEST_H_
#define _FM_TEST_H_

#include "init_parameters.h"

#define FM_AUTO_SEEK 0x0
#define FM_SET_FREQ 0x02
#define FM_POWER_ON 0x06
#define FM_POWER_OFF 0x07
#define FM_DEVICE "/dev/fm_rda5807p"
#define MAX_SEARCHING_COUNT 3
enum FM_FREQUENCY_RANGE{
    MIN_FREQUENCY = 1000,
    MAX_FREQUENCY = 1200,
};

int fm_test(struct test_Parameters *test_para);

#endif
