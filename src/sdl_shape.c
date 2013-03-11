/* ---------------------------------------------------
   shape.c

   ------------------------------------------------------*/

#include "sdl_shape.h"
#include "debug.h"

static int pen_width;  //the shape width

static Uint32 pen_color;  //the shape color
static SDL_Surface *screen = NULL;

void sdl_flip_screen(void)
{
    SDL_Flip(screen);
}
//sdl init
int init_sdl(void)
{
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
	debug_print("SDL_Init(SDL_INIT_VIDEO) FAIL!\n");
	return -1;
    }

    SDL_ShowCursor(0);

    screen = SDL_SetVideoMode( 320, 240, 32, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
	debug_print("SDL_SetVideoMode FAIL!\n");
	return -1;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
	debug_print("TTF_Init FAIL!\n");
	//        return -1;
    }

    /*font = TTF_OpenFont(FONT_PATH,18);*/
    /*background = IMG_Load(WHITE_BACKGROUND);*/
    /*lcd_test_pic = IMG_Load(LCD_TEST_PIC);*/
    /*TTF_SetFontStyle(font,TTF_STYLE_BOLD);*/
    return 0;
}
//when we draw a shape we must init this
void sdl_draw_a_pic(SDL_Surface* img, SDL_Rect *src_rect, SDL_Rect *dst_rect)
{
    SDL_BlitSurface(img,src_rect,screen,dst_rect);
}

void sdl_set_pen(SDL_Color *color, int w)
{
  pen_color  = SDL_MapRGB(screen->format, color->r, color->g, color->b);
  pen_width = w;
}	

int sdl_get_pen_width()
{
  return pen_width;
}

//draw the pixel onto the screen
static void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;

  //Here p is the address to the pixel we want to set 
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) 
  {
    case 1:
      *p = pixel;
      break;

    case 2:
      *(Uint16 *)p = pixel;
      break;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
      {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      } 
      else 
      {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;

    case 4:
      *(Uint32 *)p = pixel;
      break;
  }
}

/* rect draw functions */
void sdl_draw_rect(SDL_Rect *rect)
{
  int i;
  int j;

  for(i=0; i<rect->w; ++i)
  {
    for(j=0; j<pen_width; ++j)
    {
      put_pixel(screen, rect->x + i, rect->y + j, pen_color);
      put_pixel(screen, rect->x + i, rect->y + rect->h + j - pen_width, pen_color);
    }
  }

  for(i=0; i<rect->h; ++i)
  {
    for(j=0; j<pen_width; ++j)
    {
      put_pixel(screen, rect->x + j, rect->y + i, pen_color);
      put_pixel(screen, rect->x + j + rect->w - pen_width, rect->y + i, pen_color);
    }
  }
}

void sdl_draw_rect_solid(SDL_Rect *rect)
{
  int i;
  int j;

  for(i=0; i<rect->h; ++i)
  {
    for(j=0; j<rect->w; ++j)
    {
      put_pixel(screen, rect->x + j, rect->y + i, pen_color);
    }
  }
}

void sdl_draw_rect_dashed(SDL_Surface *screen, SDL_Rect *rect, int gap)
{
  int i;
  int j;

  if(gap > rect->w || gap > rect->h)
    return;

  for(i=0; i<rect->w; ++i)
  {
    if(i != 0 && i % gap == 0)
      continue;

    for(j=0; j<pen_width; ++j)
    {
      put_pixel(screen, rect->x + i, rect->y + j, pen_color);
      put_pixel(screen, rect->x + i, rect->y + rect->h + j, pen_color);
    }
  }

  for(i=0; i<rect->h + pen_width; ++i)
  {
    if(i != 0 && i % gap == 0)
      continue;

    for(j=0; j<pen_width; ++j)
    {
      put_pixel(screen, rect->x + j, rect->y + i, pen_color);
      put_pixel(screen, rect->x + j + rect->w, rect->y + i, pen_color);
    }
  }

}

static void plot_circle_points(SDL_Surface *screen,int xc,int yc,int x,int y,Uint32 c)
{
  put_pixel(screen, xc + x, yc + y, c);
  put_pixel(screen, xc - x, yc + y, c);
  put_pixel(screen, xc + x, yc - y, c);
  put_pixel(screen, xc - x, yc - y, c);
  put_pixel(screen, xc + y, yc + x, c);
  put_pixel(screen, xc - y, yc + x, c);
  put_pixel(screen, xc + y, yc - x, c);
  put_pixel(screen, xc - y, yc - x, c);
}

void sdl_draw_circle(SDL_Surface *screen, int xc, int yc, int radius)
{
  int i;

  for(i=0; i<pen_width; ++i)
  {
    int x;
    int y;
    int p;

    x = 0;
    y = radius;
    p = 3 - 2 * radius;

    while(x < y)
    {
      plot_circle_points(screen, xc, yc, x, y, pen_color);

      if(p < 0)
        p = p + 4 * x + 6;
      else
      {
        p = p + 4 * (x - y) + 10;
        y -= 1;
      }

      x += 1;
    }	

    if(x == y)
      plot_circle_points(screen, xc, yc, x, y, pen_color);

    ++radius;
  }

}

void sdl_draw_circle_solid(SDL_Surface *screen, int xc, int yc, int radius)
{
  for( ; radius>0; --radius)
  {
    int x;
    int y;
    int p;

    x = 0;
    y = radius;
    p = 3 - 2 * radius;

    while(x < y)
    {
      plot_circle_points(screen, xc, yc, x, y, pen_color);

      if(p < 0)
        p = p + 4 * x + 6;
      else
      {
        p = p + 4 * (x - y) + 10;
        y -= 1;
      }

      x += 1;
    }	

    if(x == y)
      plot_circle_points(screen, xc, yc, x, y, pen_color);
  }
}

void sdl_draw_rect_alpha(SDL_Surface *screen, SDL_Rect *rect, SDL_Color *color, int alpha)
{
  SDL_Surface *face_rect;
  unsigned int rm = 0xFF;
  unsigned int gm = 0xFF;
  unsigned int bm = 0xFF;
  unsigned int am = 0xFF;

  rm <<= 24;
  gm <<= 16;
  bm <<= 8;

  face_rect = SDL_CreateRGBSurface(SDL_SWSURFACE, rect->w, rect->h, 32, rm, gm, bm, am);

  SDL_FillRect(face_rect, NULL, SDL_MapRGBA(face_rect->format, color->r, color->g, color->b, alpha));

  SDL_BlitSurface(face_rect, NULL, screen, rect);
  SDL_FreeSurface(face_rect);
}
SDL_Surface *sdl_create_rect_alpha(int w,int h, SDL_Color *color, int alpha)
{
  SDL_Surface *face_rect;
  unsigned int rm = 0xFF;
  unsigned int gm = 0xFF;
  unsigned int bm = 0xFF;
  unsigned int am = 0xFF;

  rm <<= 24;
  gm <<= 16;
  bm <<= 8;

  face_rect = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rm, gm, bm, am);

  SDL_FillRect(face_rect, NULL, SDL_MapRGBA(face_rect->format, color->r, color->g, color->b, alpha));

  return face_rect;
}
SDL_Surface * sdl_create_surface(int w, int h)
{
  SDL_Surface *face_rect;
  unsigned int rm = 0xFF;
  unsigned int gm = 0xFF;
  unsigned int bm = 0xFF;
  unsigned int am = 0xFF;

  rm <<= 24;
  gm <<= 16;
  bm <<= 8;

  face_rect = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rm, gm, bm, am);

  SDL_FillRect(face_rect, NULL, SDL_MapRGBA(face_rect->format, 0xff, 0xff, 0xff, 0));

  return face_rect;
}
SDL_Surface * sdl_create_surface_with_color(int w, int h,SDL_Color *color)
{
  SDL_Surface *face_rect;
  unsigned int rm = 0xFF;
  unsigned int gm = 0xFF;
  unsigned int bm = 0xFF;
  unsigned int am = 0xFF;

  rm <<= 24;
  gm <<= 16;
  bm <<= 8;

  face_rect = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rm, gm, bm, am);

  SDL_FillRect(face_rect, NULL, SDL_MapRGBA(face_rect->format, color->r, color->g, color->b, 0xff));

  return face_rect;
}

