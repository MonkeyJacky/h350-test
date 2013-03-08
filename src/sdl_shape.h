#ifndef H_SDL_SHAPE_H
#define H_SDL_SHAOE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

void sdl_set_pen(SDL_Color *color, int w, SDL_Surface *screen);

int sdl_get_pen_width();

void sdl_draw_rect(SDL_Surface *screen, SDL_Rect *rect);

void sdl_draw_rect_solid(SDL_Surface *screen, SDL_Rect *rect);

void sdl_draw_rect_dashed(SDL_Surface *screen, SDL_Rect *rect, int gap);

void sdl_draw_circle(SDL_Surface *screen, int xc, int yc, int radius);
void sdl_draw_circle_solid(SDL_Surface *screen, int xc, int yc, int radius);

void sdl_draw_rect_alpha(SDL_Surface *screen, SDL_Rect *rect, SDL_Color *color, int alpha);

SDL_Surface *sdl_create_rect_alpha(int w,int h,SDL_Color *color, int alpha);

SDL_Surface *sdl_create_surface(int w, int h);
SDL_Surface * sdl_create_surface_with_color(int w, int h,SDL_Color *color);
#endif
