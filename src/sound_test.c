#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>

#include "init_parameters.h"
#include "debug.h"
#include "sound_test.h"

static int audio_fd = -1;
static int audio_init(void)
{
    int channels = SOUND_TRACKS;
    int format = AFMT_S16_LE;
    int speed = SAMPLINGRATE;

    if ((audio_fd = open(DSP_DEVICE, O_WRONLY)) < 0) {
	/* Open of device failed */
	perror(DSP_DEVICE);
	return False;
    }

    if (ioctl(audio_fd, SNDCTL_DSP_SETFMT, &format) == -1) {
	/* fatal error */
	perror("SNDCTL_DSP_SETFMT");
	return False;
    }

    if (format != AFMT_S16_LE) {
	/* do not support this sample mode. */
	debug_print("the format set error!\n");
	return False;
    }

    if (ioctl(audio_fd, SNDCTL_DSP_CHANNELS, &channels) == -1) {
	/* Fatal error */
	perror("SNDCTL_DSP_CHANNELS");
	return False;
    }

    if (channels != SOUND_TRACKS)
    {/* do not support stereo... */
	debug_print("the channels set error!\n");
	return False;
    }

    if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &speed)==-1) {
	/* Fatal error */
	perror("SNDCTL_DSP_SPEED");
	return False;
    }

    return True;
}

static void deinit_audio(void)
{
    if(audio_fd)
	close(audio_fd);
}

static void set_volume(int vol)
{
    int write_volume;
    int mixer = open(MIXER_DEVICE,O_WRONLY);
    if(vol > MAX_VOLUME)
	vol = MAX_VOLUME;
    if(vol < 0)
	vol = 0;

    write_volume = vol | (vol << 8);

    if (ioctl(mixer, SOUND_MIXER_WRITE_VOLUME, &write_volume) == -1)
    {
	debug_print("Failed open mixer for writting - VOLUME");
    }

    if(mixer)
	close(mixer);
}

static int audio_sound_out(char* musicfile)
{
    FILE *src_music = fopen(musicfile,"r");
    int read_count = 0;
    char audio_buffer[BUF_SIZE] = {0};
    int get_num = 0;

    if(!src_music)
    {
	debug_print("open src music file error!\n");
	return -1;
    }
    fseek(src_music,0,SEEK_END);
    rewind(src_music);

    while(feof(src_music)==0)
    {
	get_num = fread(audio_buffer,1,BUF_SIZE,src_music);
	read_count++;
	if(read_count < 10)
	    continue;

	write(audio_fd,audio_buffer,get_num);

	if(read_count > 110)
	{
	    break;
	}

	if(get_num != BUF_SIZE)
	{
	    break;
	}
    }

    if(src_music)
	fclose(src_music);
    return 0;
}

static FILE* hp_fp = NULL;
static int init_headphone(void)
{
    hp_fp = fopen(HP_PROC,"rb");
    if(!hp_fp)
    {
	debug_print("can not open hp!\n");
	return False;
    }

    return True;
}

static void deinit_headphone(void)
{
    if(hp_fp)
    {
	fclose(hp_fp);
	hp_fp = NULL;
    }
}

static int hp_detect(void)
{
    char buf[10] = {0};
    int n = 0;

    if(hp_fp)
    {
	rewind(hp_fp);
	fgets(buf,9,hp_fp);
	sscanf(buf,"%d",&n);
    }

    return n;
}

int speaker_test(struct test_Parameters *test_para)
{
    int speaker_test_flag = 0;
    if(audio_init() < 0)
	return False;

    set_volume(50);
    audio_sound_out(TEST_SOUND);
    //TODO get sound_test_flag 

    deinit_audio();

    return speaker_test_flag;
}

int hp_test(struct test_Parameters *test_para)
{
    int hp_test_flag = 0;
    int hp_loop = 1;
    int audio_out_count = 0;

    if(audio_init() < 0 || init_headphone() < 0)
	return False;

    set_volume(50);
    while(hp_loop)
    {
	if(hp_detect() == 1)
	{
	    if(audio_out_count > WAIT_COUNTS)
	    {
		audio_sound_out(TEST_SOUND);
		//TODO get hp_test_flag
		hp_loop = 0;
	    }

	    audio_out_count ++;
	}

	usleep(20*1000);
    }

    deinit_headphone();
    return hp_test_flag;
}
