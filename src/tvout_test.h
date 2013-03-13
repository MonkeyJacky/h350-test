#ifndef _TVOUT_TEST_H_
#define _TVOUT_TEST_H_

#include "init_parameters.h"

#define PANEL_MODE_LCD 0
#define HDMI_SET_OFF 0
#define HDMI_SET_ON 1
#define DEFAULT_HDMI_MODE 3
#define HDMI_IC_DEV "/proc/jz/it6610_me"
#define HDMI_PROC_DEV "/proc/jz/hdmi_me"
#define AVOUT_DEV "/proc/jz/tvout"
#define FRAME_BUFFER_DEV "/dev/fb0"
#define FBIO_MODE_SWITCH 0x46a5

int hdmi_test(struct test_Parameters *test_para);
int avout_test(struct test_Parameters *test_para);

#endif
