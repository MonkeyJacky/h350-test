/* ---------------------------------------------------
draw interface.
   ------------------------------------------------------*/

#include "sdl_interface.h"
#include "debug.h"
#include "init_parameters.h"

static int pen_width;  //the shape width

static Uint32 pen_color;  //the shape color
static SDL_Surface *screen = NULL;
static SDL_Surface *background = NULL;
static SDL_Surface *pass_view = NULL;
static SDL_Surface *fail_view = NULL;
static TTF_Font *font;

void sdl_flip_screen(void)
{
    SDL_Flip(screen);
}

void sdl_free_surface(SDL_Surface* obj_surface)
{
    if(obj_surface)
    {
	SDL_FreeSurface(obj_surface);
	obj_surface = NULL;
    }
}

void draw_decision_pic(int sel)
{
    if(sel == PASS)
    {
	SDL_BlitSurface(pass_view,NULL,screen,NULL);
    }
    else
    {
	SDL_BlitSurface(fail_view,NULL,screen,NULL);
    }

    sdl_flip_screen();
    SDL_Delay(1000);
}

void test_words_show(const char* temp_str, SDL_Color color)
{
	SDL_Rect test_words_rect;
	SDL_Surface* temp_test_words =
		TTF_RenderUTF8_Blended(font,temp_str,color);

	test_words_rect.x = (screen->w - temp_test_words->w)/2;
	test_words_rect.y = (screen->h - temp_test_words->h)/2 - 20;

	SDL_BlitSurface(background,NULL,screen,NULL);
	SDL_BlitSurface(temp_test_words,NULL,screen,&test_words_rect);

	sdl_flip_screen();

	sdl_free_surface(temp_test_words);

	SDL_Delay(1000);
}

SDL_Surface* load_image(char* img_path)
{
    SDL_Surface* img = IMG_Load(img_path);
    if(!img)
    {
	debug_print("init %s error!\n",img_path);
	return NULL;
    }

    return img;
}
void deinit_sdl(void)
{
    sdl_free_surface(screen);
    sdl_free_surface(background);
    sdl_free_surface(pass_view);
    sdl_free_surface(fail_view);
    SDL_Quit();
}

static void result_res_init(struct test_Parameters *test_para, struct _result_res *result_res)
{
    int i;

    result_res->Fail_color.r =
	test_para->result_view_para.fail_color[0];
    result_res->Fail_color.g =
	test_para->result_view_para.fail_color[1];
    result_res->Fail_color.b =
	test_para->result_view_para.fail_color[2];

    result_res->Pass_color.r =
	test_para->result_view_para.pass_color[0];
    result_res->Pass_color.g =
	test_para->result_view_para.pass_color[1];
    result_res->Pass_color.b =
	test_para->result_view_para.pass_color[2];

    result_res->Untest_color.r =
	test_para->result_view_para.untest_color[0];
    result_res->Untest_color.g =
	test_para->result_view_para.untest_color[1];
    result_res->Untest_color.b =
	test_para->result_view_para.untest_color[2];

    result_res->font = TTF_OpenFont(FONT_PATH,13);

    result_res->result_string =
	malloc(sizeof(SDL_Surface *) * test_para->total_num);
    result_res->item_string =
	malloc(sizeof(SDL_Surface *) * test_para->total_num);

    for(i = 0; i < test_para->total_num; i++)
    {
	if(test_para->result_flag[i] == PASS)
	{
	    result_res->item_string[i] =
		TTF_RenderUTF8_Blended(result_res->font,test_para->test_order[i],result_res->Pass_color);
	    result_res->result_string[i] =
		TTF_RenderUTF8_Blended(result_res->font,test_para->result_view_para.result_words[1],result_res->Pass_color);
	}
	else if(test_para->result_flag[i] == FAIL)
	{
	    result_res->item_string[i] =
		TTF_RenderUTF8_Blended(result_res->font,test_para->test_order[i],result_res->Fail_color);
	    result_res->result_string[i] =
		TTF_RenderUTF8_Blended(result_res->font,test_para->result_view_para.result_words[0],result_res->Fail_color);
	}
	else if(test_para->result_flag[i] == UNTEST)
	{
	    result_res->item_string[i] =
		TTF_RenderUTF8_Blended(result_res->font,test_para->test_order[i],result_res->Untest_color);
	    result_res->result_string[i] =
		TTF_RenderUTF8_Blended(result_res->font,test_para->result_view_para.result_words[2],result_res->Untest_color);
	}
    }
}

static void result_res_deinit(struct test_Parameters *test_para, struct _result_res *result_res)
{
    int i;

    for(i = 0; i < test_para->total_num; i++)
    {
	sdl_free_surface(result_res->item_string[i]);
	sdl_free_surface(result_res->result_string[i]);
    }

    deep_free(result_res->item_string);
    deep_free(result_res->result_string);
}

void result_show(struct test_Parameters *test_para)
{
    struct _result_res result_res;
    SDL_Rect tmp_rect = {0,0,0,0};
    SDL_Rect tmp_result_rect = {200,0,0,0};
    int i;

    result_res_init(test_para,&result_res);

    SDL_BlitSurface(background,NULL,screen,NULL);
    for(i = 0; i<test_para->total_num; i++)
    {
	SDL_BlitSurface(result_res.item_string[i],NULL,screen,&tmp_rect);
	SDL_BlitSurface(result_res.result_string[i],NULL,screen,&tmp_result_rect);
	tmp_rect.y += 10;
	tmp_result_rect.y += 10;
    }

    sdl_flip_screen();

    result_res_deinit(test_para,&result_res);
}
//sdl init
int init_sdl(struct test_Parameters *test_para)
{
    if( SDL_Init( SDL_INIT_VIDEO ) == False )
    {
	debug_print("SDL_Init(SDL_INIT_VIDEO) FAIL!\n");
	return False;
    }

    SDL_ShowCursor(0);

    screen = SDL_SetVideoMode( test_para->screen_info.width, test_para->screen_info.height, test_para->screen_info.bits_per_pixel, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
	debug_print("SDL_SetVideoMode FAIL!\n");
	return False;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == False )
    {
	debug_print("TTF_Init FAIL!\n");
	return False;
    }

    font = TTF_OpenFont(FONT_PATH,15);
    if( !(background = load_image(WHITE_BACKGROUND)) )
	return False;
    if( !(pass_view = load_image(PASS_VIEW)) )
	return False;
    if( !(fail_view = load_image(FAIL_VIEW)) )
	return False;
    /*lcd_test_pic = IMG_Load(LCD_TEST_PIC);*/
    TTF_SetFontStyle(font,TTF_STYLE_BOLD);
    return True;
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

