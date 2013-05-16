#ifndef _SOUND_TEST_H_
#define _SOUND_TEST_H_

#include "init_parameters.h"

#define BUF_SIZE 4096
#define SOUND_TRACKS 2
#define MAX_VOLUME 100
#define WAIT_COUNTS 20
#define MIXER_DEVICE "/dev/mixer"
#define SAMPLINGRATE 44100
#define RECORDER_SAMPLINGRATE 8000
#define DSP_DEVICE "/dev/dsp"
#define HP_PROC "/proc/jz/hp_l009"
#define TEST_SOUND "res/test_sound.wav"
#define RECORD_FILE "/mnt/memory/record_file.wav"

#define RECORD_TIME 3
#define	RIFF 0x46464952	
#define	WAVE 0x45564157
#define	FMT 0x20746D66
#define	DATA 0x61746164
#define WAVE_RIFF_FLEN 8
#define	PCM_CODE 1

typedef struct _waveheader {
  u_long  main_chunk;		/* 'RIFF' */
  u_long  length;			/* filelen */
  u_long  chunk_type;		/* 'WAVE' */

  u_long  sub_chunk;		/* 'fmt ' */
  u_long  sc_len;			/* length of sub_chunk, =16 */
  u_short format;			/* should be 1 for PCM-code */
  u_short modus;			/* 1 Mono, 2 Stereo */
  u_long  sample_fq;		/* frequence of sample */
  u_long  byte_p_sec;
  u_short byte_p_spl;		/* samplesize; 1 or 2 bytes */
  u_short bit_p_spl;		/* 8, 12 or 16 bit */ 

  u_long  data_chunk;		/* 'data' */
  u_long  data_length;		/* samplecount */
} WaveHeader;

int speaker_test(struct test_Parameters *test_para);
int hp_test(struct test_Parameters *test_para);
int record_test(struct test_Parameters *test_para);
void set_volume(int vol);
int audio_sound_out(char* musicfile);
void deinit_audio(void);
int audio_init(int sampling_rate, int mode);
void enable_fm(int mode);
int hp_detect(void);
void deinit_headphone(void);
int init_headphone(void);

#endif
