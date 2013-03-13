#ifndef _KEY_TEST_H_
#define _KEY_TEST_H_

#include "init_parameters.h"

#define KEYPAD "/dev/keypad"
#define WHITE_BACKGROUND "res/white_back.png"

int key_test(struct test_Parameters *test_para);

int joystick_test(struct test_Parameters *test_para);

#endif
