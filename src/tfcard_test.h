#ifndef _TFCARD_TEST_H_
#define _TFCARD_TEST_H_

#include "init_parameters.h"

#define TFCARD_DEVICE "/var/MMCNAME"
#define CHECK_TIMES 10
#define NOTICE_FILE "/mnt/mmc/emus"
#define ENOUGH_TIMES 3

int tfcard_test(struct test_Parameters *test_para);

#endif
