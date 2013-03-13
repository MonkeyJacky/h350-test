#ifndef _INTERNAL_TEST_H_
#define _INTERNAL_TEST_H_

#include "init_parameters.h"

#define _20MB_ (20*1024*1024)
#define TEST_FILE "/mnt/memory/test_file"
#define MEMORY_PATH "/mnt/memory"
#define BLOCK0P4 "/dev/mmcblk0p4"

int internal_card_test(struct test_Parameters *test_para);

#endif
