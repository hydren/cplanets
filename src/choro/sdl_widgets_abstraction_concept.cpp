/*
 * sdl_widgets_abstraction_concept.cpp
 *
 *  Created on: 28 de jan de 2016
 *      Author: carlosfaruolo
 */

#include "sdl-widgets/sdl-widgets.h"

struct CustomButton : public Button
{
	public:
	CustomButton(WinBase *pw,Style st,Rect rect,Label lab,void (*_cmd)(Button*),Id _id=0):
		Button(pw, st, rect, lab, _cmd, _id)
	{}
	void custom_cmd() {  }
};

TopWin *top_win1;
CustomButton *but1;

void topw_disp1()
{
  top_win1->clear();
  draw_title_ttf->draw_string(top_win1->win,"Hello world!",Point(20,40));
}

void button_cmd1(Button* b)
{
  top_win1->clear(&b->tw_area,cBackground,true);
  static int y=10;
  y= y==10 ? 70 : 10;
  b->area.y=b->tw_area.y=y;
  ((CustomButton*) b)->custom_cmd();
}

void runExample()
{
	top_win1=new TopWin("Hello",Rect(100,100,120,100),SDL_INIT_VIDEO,0,topw_disp1);
	but1=new CustomButton(top_win1,0,Rect(5,10,60,0),"catch me!",button_cmd1);
	get_events();
}
