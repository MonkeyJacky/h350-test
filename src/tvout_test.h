#ifndef _TVOUT_TEST_H_
#define _TVOUT_TEST_H_

#include "init_parameters.h"

#define SET_OFF 0
#define SET_ON 1
#define HDMI_IC_DEV "/proc/jz/it6610_me"
#define HDMI_PROC_DEV "/proc/jz/hdmi_me"
#define AVOUT_SWITCH_DEV "/proc/jz/tvout_me"
#define AVOUT_DEV "/proc/jz/hp_l009"
#define FBIO_MODE_SWITCH 0x46a5
#define TEST_BACK_IMG "res/test_pic.png"
enum TVOUT_MODE_ARRAY{LCD_MODE,AVOUT_PAL_MODE,AVOUT_NTSC_MODE,HDMI_MODE};

int hdmi_test(struct test_Parameters *test_para);
int avout_test(struct test_Parameters *test_para);

#endif
