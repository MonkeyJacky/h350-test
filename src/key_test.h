#ifndef _KEY_TEST_H_
#define _KEY_TEST_H_

#include "init_parameters.h"

#define KEYPAD "/dev/keypad"

int key_test(struct test_Parameters *test_para);

int joystick_test(struct test_Parameters *test_para);

#endif
