#ifndef _KEY_TEST_H_
#define _KEY_TEST_H_

#include "init_parameters.h"

#define KEYPAD "/dev/keypad"
#define OTG_DETECT_PROC "/proc/jz/otg_state"

int key_test(struct test_Parameters *test_para);

void wait_for_next(void);
int press_B_to_quit(void);
int joystick_test(struct test_Parameters *test_para);
void init_key_pad(void);
int decision_loop(void);
void deinit_key_pad(void);
int press_A_go_on(void);
int select_test_key_loop(struct test_Parameters *test_para);
int gsensor_test_loop(struct test_Parameters *test_para);
int otg_test_loop(struct test_Parameters *test_par);

#endif
