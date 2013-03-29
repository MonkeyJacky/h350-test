#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL.h>

#include "internal_card_test.h"
#include "init_parameters.h"
#include "sdl_interface.h"
#include "debug.h"
static SDL_Color Bcolor = {0,0,0};
static FILE* card_fp = NULL;
static void write_test_file(int write_length)
{
	card_fp = fopen(TEST_FILE,"w+");
	char each_write_length[20];
	int return_write = 0;
	int count = 0;
	sprintf(each_write_length,"justfortest");

	rewind(card_fp);
	do
	{
		return_write = fwrite(each_write_length,sizeof(unsigned char),sizeof(unsigned char)*20,card_fp);
		count++;
	}while(return_write*count < write_length);

	sync();
	fclose(card_fp);
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
    write_test_file(_20MB_);
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
	sync();
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
