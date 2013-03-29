#ifndef H_SDL_SHAPE_H
#define H_SDL_SHAOE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "init_parameters.h"

#ifdef H350
#define FONT_PATH "/usr/share/font/l009.ttf"
#else
#define FONT_PATH "res/l009.ttf"
#endif

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

#endif
