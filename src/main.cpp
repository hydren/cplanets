/*
 * main.cpp
 *
 *  Created on: 02/03/2015
 *      Author: Carlos F. M. Faruolo <5carlosfelipe5@gmail.com>
 */

#include "sdl-widgets/sdl-widgets.h"
#include <cstdlib>
#include "geometry/vector2d.hpp"

#ifdef _WIN32
	#define SDLMAIN_STREAM_WORKAROUND workaround_sdl_stream_file = fopen("CON", "w" ); freopen( "CON", "w", stdout ); freopen( "CON", "w", stderr )
#else
	#define SDLMAIN_STREAM_WORKAROUND ;
#endif

TopWin *top_win;
Button *but;

FILE* workaround_sdl_stream_file = NULL;// part of workaround

void topw_disp()
{
  top_win->clear();
  draw_title_ttf->draw_string(top_win->win,"Hello world!",Point(20,40));
}

void button_cmd(Button* b)
{
  top_win->clear(&b->tw_area,cBackground,true);
  static int y=10;
  y= y==10 ? 70 : 10;
  b->area.y=b->tw_area.y=y;
}

int main(int arc, char* argv[])
{
  SDLMAIN_STREAM_WORKAROUND;// part of workaround
  puts("Program start");
  top_win=new TopWin("Hello",Rect(100,100,120,100),SDL_INIT_VIDEO,0,topw_disp);
  but=new Button(top_win,0,Rect(5,10,60,0),"catch me!",button_cmd);
  get_events();
  if(workaround_sdl_stream_file != NULL) fclose(workaround_sdl_stream_file); // part of workaround
  return EXIT_SUCCESS;
}






