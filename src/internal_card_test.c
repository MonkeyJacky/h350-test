#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL.h>

#include "internal_card_test.h"
#include "init_parameters.h"
#include "debug.h"
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

int internal_card_test()
{
    int start = 0,last = 0;
    char temp_command[MAX_SIZE] = {0};

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"umount %s",MEMORY_PATH);
    system(temp_command);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"mkfs.vfat %s",BLOCK0P4);
    system(temp_command);
    
    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"mount %s %s",BLOCK0P4,MEMORY_PATH);
    system(temp_command);

    start = SDL_GetTicks();
    write_test_file(_20MB_);
    last = SDL_GetTicks() - start;
    debug_print("%s %d lasttime is %d\n",__FILE__,__LINE__,last);

    memset(temp_command,0,MAX_SIZE);
    sprintf(temp_command,"rm %s -r",TEST_FILE);
    system(temp_command);
    sync();

    if(last < 10000 && last > 1000)
	return True;
    else
	return False;
}
