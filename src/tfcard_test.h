#ifndef _TFCARD_TEST_H_
#define _TFCARD_TEST_H_

#define TFCARD_DEVICE "/var/MMCNAME"
#define CHECK_TIMES 10
#define NOTICE_FILE "/mnt/mmc/test_mode.txt"
#define ENOUGH_TIMES 3

int tfcard_test(void);

#endif
