#ifndef _KEY_TEST_H_
#define _KEY_TEST_H_

#include "init_parameters.h"

#define KEYPAD "/dev/keypad"

int key_test(struct test_Parameters *test_para);

int press_B_to_quit(void);
int joystick_test(struct test_Parameters *test_para);
void init_key_pad(void);
int decision_loop(void);
void deinit_key_pad(void);

#endif
