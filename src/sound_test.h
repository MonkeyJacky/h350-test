#ifndef _SOUND_TEST_H_
#define _SOUND_TEST_H_

#include "init_parameters.h"

#define BUF_SIZE (2*2*735)
#define SOUND_TRACKS 2
#define MAX_VOLUME 100
#define WAIT_COUNTS 10
#define MIXER_DEVICE "/dev/mixer"
#define SAMPLINGRATE 44100
#define DSP_DEVICE "/dev/dsp"
#define HP_PROC "/proc/jz/hp_l009"
#define TEST_SOUND "res/test_sound.wav"

int speaker_test(struct test_Parameters *test_para);
int hp_test(struct test_Parameters *test_para);
#endif
