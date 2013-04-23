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
#include "key_test.h"
#include "sdl_interface.h"

static SDL_Color Bcolor = {0,0,0};
static int audio_fd = -1;
int audio_init(int sampling_rate, int mode)
{
    int channels = SOUND_TRACKS;
    int format = AFMT_S16_LE;
    int speed = sampling_rate;

    if ((audio_fd = open(DSP_DEVICE, mode, 0)) < 0) {
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

    if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &speed)==-1) {
	/* Fatal error */
	perror("SNDCTL_DSP_SPEED");
	return False;
    }

    return True;
}

void deinit_audio(void)
{
    if(audio_fd)
    {
	close(audio_fd);
	audio_fd = -1;
    }
}

void set_volume(int vol)
{
    int write_volume;
    int mixer = open(MIXER_DEVICE,O_WRONLY);
    if(mixer < 0)
    {
	debug_print("open %s error!\n",MIXER_DEVICE);
	return;
    }
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

static void set_mic_volume(void)
{
    int vol = 100;
    int mixer_fd = open(MIXER_DEVICE,O_WRONLY);
    if(mixer_fd == -1)
    {
	debug_print("open %s error\n",MIXER_DEVICE);
	return;
    }

    if(ioctl(mixer_fd,SOUND_MIXER_WRITE_MIC,&vol) == -1)
    {
	debug_print("Failed write mic volume\n");
    }

    close(mixer_fd);
}

#define TEST_BUF 1024
int audio_sound_out(char* musicfile)
{
    FILE *src_music = fopen(musicfile,"r");
    int read_count = 0;
    char audio_buffer[TEST_BUF] = {0};
    int get_num = 0;
    int write_num = 0;

    if(!src_music)
    {
	debug_print("open src music file error!\n");
	return -1;
    }
    fseek(src_music,0,SEEK_END);
    rewind(src_music);

    while(feof(src_music)==0)
    {
	get_num = fread(audio_buffer,1,TEST_BUF,src_music);
	if(ferror(src_music))
	    return -1;
	read_count++;
	if(read_count < 5)//Initially there will be sonic boom.
	    continue;

	write_num = write(audio_fd,audio_buffer,get_num);
	if(write_num == 0)
	{
	    debug_print("write audio error\n");
	    break;
	}
    }

    if(src_music)
	fclose(src_music);
    return 0;
}

static FILE* hp_fp = NULL;
int init_headphone(void)
{
    hp_fp = fopen(HP_PROC,"rw");
    if(!hp_fp)
    {
	debug_print("can not open hp!\n");
	return False;
    }

    return True;
}

void enable_fm(int mode)
{
    char temp_command[MAX_SIZE] = {0};
    sprintf(temp_command,"echo %d > %s",mode,HP_PROC);
    system(temp_command);
}

void deinit_headphone(void)
{
    if(hp_fp)
    {
	fclose(hp_fp);
	hp_fp = NULL;
    }
}

int hp_detect(void)
{
    char buf[10] = {0};
    int n = 0;

    if(hp_fp)
    {
	rewind(hp_fp);
	if(!fgets(buf,9,hp_fp))
	    return 0;
	sscanf(buf,"%d",&n);
    }

    return n;
}

int speaker_test(struct test_Parameters *test_para)
{
    int speaker_test_flag = False;
#ifdef H350
    set_volume(100);

    if(audio_init(SAMPLINGRATE, O_WRONLY) < 0)
	return False;
#endif
    test_words_show("Speaker sound test",Bcolor);
    test_words_show("Play now....",Bcolor);
#ifdef H350
    audio_sound_out(TEST_SOUND);
#endif
    speaker_test_flag = decision_loop();

#ifdef H350
    deinit_audio();
#endif

    return speaker_test_flag;
}

int hp_test(struct test_Parameters *test_para)
{
    int hp_test_flag = False;
    int hp_loop = 1;
    int audio_out_count = 0;

#ifdef H350
    if(audio_init(SAMPLINGRATE, O_WRONLY) < 0 || init_headphone() < 0)
	return False;

    set_volume(70);
#endif
    test_words_show("Headphone sound test",Bcolor);
    test_words_show("Please insert the headphone...",Bcolor);
#ifdef H350
    while(hp_loop)
    {
	if(hp_detect() == 1)
	{
	    if(audio_out_count > WAIT_COUNTS)
	    {
		test_words_show("Play now....",Bcolor);
		audio_sound_out(TEST_SOUND);
		test_words_show("Pull out the headphone to go on",Bcolor);
		while(hp_loop)
		{
		    if(hp_detect() == 0)
		    {
			hp_loop = 0;
			hp_test_flag = decision_loop();
		    }

		    usleep(20*1000);
		}
	    }

	    audio_out_count ++;
	}

	usleep(20*1000);
    }

    deinit_headphone();
    deinit_audio();
#endif
    return hp_test_flag;
}
//record test
int start_record(char* record_file)
{
    unsigned char audio_buffer[BUF_SIZE] = {0};
    int len = 0;
    int bytp_p_sec = 0;
    FILE *fp_wave = NULL;
    fp_wave = fopen(record_file,"wb");
    if(!fp_wave)
    {
	system("mkfs.vfat /dev/mmcblk0p4 -I");
	fp_wave = NULL;
	fp_wave = fopen(record_file,"wb");
	if(!fp_wave)
	    return False;
    }
    WaveHeader wh;
    int dsp_speed = RECORDER_SAMPLINGRATE;
    int dsp_stereo = 1;
    unsigned long samplesize = 16;
    int cnt = 0;
    fwrite(&wh,sizeof(WaveHeader),1,fp_wave);

    wh.main_chunk = RIFF;
    wh.length= cnt+ sizeof(WaveHeader) - WAVE_RIFF_FLEN;
    wh.chunk_type = WAVE;
    wh.sub_chunk = FMT;
    wh.sc_len = 16;
    wh.format = PCM_CODE;
    wh.modus = dsp_stereo ? 2 :1;
    wh.sample_fq = dsp_speed;
    wh.byte_p_spl = ( (samplesize == 8) ? 1 : 2) * (dsp_stereo ? 2 :1);
    bytp_p_sec = wh.byte_p_sec = dsp_speed * wh.modus * wh.byte_p_spl/2;

    wh.bit_p_spl = samplesize;
    wh.data_chunk = DATA;
    wh.data_length = cnt;
    int buffer_len = 0;
    int run = 1;
    int cur_time = 0;
    while(run)
    {
	if ((len = read(audio_fd, audio_buffer, BUF_SIZE)) == -1) 
	{
	    perror("audio read");
	    run = 0;
	}

	buffer_len = fwrite(audio_buffer,len,1,fp_wave);
	if(buffer_len == 0)
	{
	    perror("write file error\n");
	    run = 0;
	}

	cnt+=len;
	cur_time = 1*cnt/bytp_p_sec;
	if(cur_time > RECORD_TIME - 1)//recording 5 seconds
	    run = 0;
    }

    wh.length= cnt+ sizeof(WaveHeader) - WAVE_RIFF_FLEN;
    wh.data_length = cnt;
    fseek(fp_wave,0,SEEK_SET);
    fwrite(&wh,sizeof(WaveHeader),1,fp_wave);

    printf("record end\n");
    fflush(fp_wave);
    fsync(fileno(fp_wave));
    fclose(fp_wave);
    return 0;
}

int record_test(struct test_Parameters *test_para)
{
    test_words_show("Recorder test",Bcolor);

    set_mic_volume();

    if(audio_init(RECORDER_SAMPLINGRATE, O_RDONLY) == False)
    {
	debug_print("audio init error!\n");
	return False;
    }
    test_words_show("Recording now....Say something",Bcolor);
    if(start_record(RECORD_FILE) == False)
	return False;
    deinit_audio();

    set_volume(100);
    if(audio_init(RECORDER_SAMPLINGRATE, O_WRONLY) == False)
    {
	debug_print("audio init error!\n");
	return False;
    }
    test_words_show("Play the recording....",Bcolor);
    audio_sound_out(RECORD_FILE);
    deinit_audio();

    remove(RECORD_FILE);

    return decision_loop();
}
