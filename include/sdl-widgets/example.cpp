/*
 * example.cpp
 *
 *  Created on: 19/06/2015
 *      Author: Carlos Faruolo
 */

#include "sdl-widgets/example.hpp"

TopWin *top_win;
Button *but;

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

void Widgets_RunExample()
{
	top_win=new TopWin("Hello",Rect(100,100,120,100),SDL_INIT_VIDEO,0,topw_disp);
	but=new Button(top_win,0,Rect(5,10,60,0),"catch me!",button_cmd);
	get_events();
}