#ifndef H_SDL_SHAPE_H
#define H_SDL_SHAOE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "init_parameters.h"

#define FONT_PATH "res/l009.ttf"
#define PAGE_MAX_ITEM 11

struct _result_res{
    SDL_Color Fail_color;
    SDL_Color Pass_color;
    SDL_Color Untest_color;
    TTF_Font *font;
    TTF_Font *focus_font;
    SDL_Surface **item_string;
    SDL_Surface **result_string;
};

int init_sdl(struct test_Parameters *test_para);
void deinit_sdl(void);
void draw_decision_pic(int sel);
SDL_Surface* load_image(char* img_path);
void sdl_free_surface(SDL_Surface* obj_surface);
void test_words_show(const char* temp_str, SDL_Color color);
void sdl_draw_a_pic(SDL_Surface* img, SDL_Rect *src_rect, SDL_Rect *dst_rect);
void sdl_flip_screen(void);
void sdl_set_pen(SDL_Color *color, int w);
int sdl_get_pen_width();
void sdl_draw_rect(SDL_Rect *rect);
void sdl_draw_rect_solid(SDL_Rect *rect);
void result_show(struct test_Parameters *test_para);

#endif
