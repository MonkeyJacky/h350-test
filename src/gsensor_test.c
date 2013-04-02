#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "gsensor_test.h"
#include "key_test.h"
#include "sdl_interface.h"

static SDL_Color Bcolor = {0,0,0};

int gsensor_test(struct test_Parameters *test_para)
{
    char tmp_command[MAX_SIZE] = {0};
    int gsensor_result = FAIL;

#ifdef H350
    //open gsensor.
    test_words_show("Gsensor test",Bcolor);
    memset(tmp_command,0,MAX_SIZE);
    sprintf(tmp_command,"echo 1 > %s",GSENSOR_DEV);
    system(tmp_command);
#endif

    gsensor_result = gsensor_test_loop(test_para);

#ifdef H350
    //close gsensor.
    memset(tmp_command,0,MAX_SIZE);
    sprintf(tmp_command,"echo 0 > %s",GSENSOR_DEV);
    system(tmp_command);
#endif

    return gsensor_result;
}
