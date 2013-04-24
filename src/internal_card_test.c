#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL.h>

#include "internal_card_test.h"
#include "init_parameters.h"
#include "sdl_interface.h"
#include "debug.h"
static SDL_Color Bcolor = {0,0,0};
int write_test_file(char* test_file,int write_length)
{
    FILE* card_fp = NULL;
    card_fp = fopen(test_file,"w+");
    char each_write_length[20];
    int return_write = 0;
    int count = 0;
    sprintf(each_write_length,"justfortest");
    if(!card_fp)
    {
	debug_print("open file error!\n");
	return False;
    }

    rewind(card_fp);
    do
    {
	return_write = fwrite(each_write_length,sizeof(unsigned char),sizeof(unsigned char)*20,card_fp);
	if (return_write == 0)
	{
	    debug_print("write error!\n");
	    return False;
	}

	count++;
    }while(return_write*count < write_length);

    fflush(card_fp);
    fsync(fileno(card_fp));
    fclose(card_fp);

    return True;
}

int internal_card_test(struct test_Parameters *test_para)
{
    int start = 0,last = 0;
    char temp_command[MAX_SIZE] = {0};
    int ret = 0;

    test_words_show("Internal card writting test",Bcolor);

    test_words_show("Writting now...",Bcolor);
    start = SDL_GetTicks();
#ifdef H350

#if 0
    while(1)
    {
	start = SDL_GetTicks();
	if (write_test_file(TEST_FILE,_40MB_) == False)
	{
	    system("mkfs.vfat /dev/mmcblk0p4 -I");
	    start = SDL_GetTicks();
	    if(write_test_file(TEST_FILE,_40MB_) == False)
	    {
		draw_decision_pic(FAIL);
		return False;
	    }
	}

	last = SDL_GetTicks() - start;
	memset(temp_command,0,MAX_SIZE);
	sprintf(temp_command,"Writting interval %0.2f S",(float)last/1000);
	test_words_show(temp_command,Bcolor);
	sleep(1);
	test_words_show("Test again....",Bcolor);
    }
#else
    if (write_test_file(TEST_FILE,_20MB_) == False)
    {
	system("unmount /mnt/memory/");
	system("mkfs.vfat /dev/mmcblk0p4 -I"); //add '-I' parameter, if the card is partitioned, will get an error.
	system("mount /dev/mmcblk0p4 /mnt/memory");
	start = SDL_GetTicks();
	if(write_test_file(TEST_FILE,_20MB_) == False)
	{
	    draw_decision_pic(FAIL);
	    return False;
	}
    }
#endif

#endif
    last = SDL_GetTicks() - start;
    debug_print("%s %d lasttime is %d\n",__FILE__,__LINE__,last);

#ifdef H350
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"rm %s",TEST_FILE);
    ret = system(temp_command);
#endif

    if(ret < 0){
	debug_print("rm %s error",TEST_FILE);
    }else{
#ifdef H350
	/*sync();*/
#endif
    }

    if(last < 10000 && last > 1000)
    {
	draw_decision_pic(True);
	return True;
    }
    else
    {
	draw_decision_pic(FAIL);
    }

    return False;
}
