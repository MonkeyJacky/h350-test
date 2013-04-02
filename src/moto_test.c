#include <stdlib.h>
#include <unistd.h>
#include "moto_test.h"
#include "sdl_interface.h"
#include "key_test.h"

static SDL_Color Bcolor = {0,0,0};
static void vibration(void)
{
    char tmp_command[MAX_SIZE] = {0};

    sprintf(tmp_command,"echo 1 > %s",MOTO_DEV);
    system(tmp_command);
}

static void stop_vibration(void)
{
    char tmp_command[MAX_SIZE] = {0};

    sprintf(tmp_command,"echo 0 > %s",MOTO_DEV);
    system(tmp_command);
}

int moto_test(struct test_Parameters *test_para)
{
    test_words_show("Moto vibration test",Bcolor);
#ifdef H350
    vibration();
    sleep(3);
    stop_vibration();
#endif
    return decision_loop();
}
