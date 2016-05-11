/*
 * widgets_util.cpp
 *
 *  Created on: 27 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "widgets_util.hpp"

#include "futil/futil.hpp"

void WidgetsExtra::setComponentPosition(WinBase* component, const Point& position)
{
	setComponentPosition(component, position.x, position.y);
}

void WidgetsExtra::setComponentPosition(WinBase* component, int x, int y)
{
	setComponentPositionX(component, x);
	setComponentPositionY(component, y);
}

void WidgetsExtra::setComponentPositionX(WinBase* component, int x)
{
	component->area.x = x;
	component->tw_area.x = x;
	component->title_area.x = x;
	component->title_top.x = x;
	if (component->parent)
	{
		component->tw_area.x=component->area.x+component->parent->tw_area.x;
	}
}

void WidgetsExtra::setComponentPositionY(WinBase* component, int y)
{
	component->area.y = y;
	component->tw_area.y = y;
	component->title_area.y = y-17;
	component->title_top.y = y-17;
	if (component->parent)
	{
		component->tw_area.y=component->area.y+component->parent->tw_area.y;
	}
}

void WidgetsExtra::packLabeledComponent(WinBase* wb, Label& lbl, unsigned height, int padding)
{
	int properWidth = lbl.render_t->text_width(lbl.str) + 2*padding;
	int properHeight = height > 0? height : TTF_FontHeight(lbl.render_t->ttf_font) + 2*padding;  //if height is 0, use font height plus padding instead
	wb->widen(properWidth - wb->tw_area.w, properHeight - wb->tw_area.h);
}

void WidgetsExtra::packLabeledComponent(Button* btn, unsigned height, int padding)
{
	packLabeledComponent(btn, btn->label, height, padding);
}

void WidgetsExtra::packLabeledComponent(CheckBox* btn, unsigned height, int padding)
{
	int properHeight = height > 0? height : 14;  //if height is 0, use size 14 (check symbol size defined as pixmap)
	int properWidth = btn->label.render_t->text_width(btn->label.str) + 2*padding + properHeight;
	btn->widen(properWidth - btn->tw_area.w, properHeight - btn->tw_area.h);
}

void WidgetsExtra::packLabeledComponent(RExtButton* btn, unsigned height, int padding)
{
	packLabeledComponent(btn, btn->label, height, padding);
}

void WidgetsExtra::drawDefaultBgrWin(BgrWin* bgrWin)
{
	bgrWin->init_gui();
	SDL_FillRect(bgrWin->win, null, bgrWin->bgcol); //clears the screen
}

WidgetsExtra::Color5::Color5(Uint32 fst, Uint32 lst)
: ready(false)
{
	this->c[0] = fst;
	this->c[4] = lst;
}

void WidgetsExtra::Color5::set_col(Uint32 fst,Uint32 lst) { // fst, lst e.g.: 0x102030ff, output c[i] e.g: 0x102030
  for (int i=0;i<5;++i) {
    Uint32 cc=0;
    for (int cn=0;cn<3;++cn) {  // cn = red,green,blue
      Uint8 col=((fst>>(cn+1)*8 & 0xff) * (4-i) + (lst>>(cn+1)*8 & 0xff)*i) / 4;
      cc += col<<cn*8;
    }
    c[i]=calc_color(cc);
  }
  ready = true;
}

void WidgetsExtra::Color5::draw_gradient(WinBase* self, Rect rect, bool vertical, bool hollow) const
{
	const_cast<Color5*>(this)->setup();
	const int x[6]={ 0,rect.w/5,rect.w*2/5,rect.w*3/5,rect.w*4/5,rect.w },
			y[6]={ 0,rect.h/5,rect.h*2/5,rect.h*3/5,rect.h*4/5,rect.h };
	for (int i=0;i<5;++i) {
		Uint32 c= hollow ? this->c[4-i] : this->c[i];
		if (vertical)
			SDL_FillRect(self->win,rp(rect.x+x[i],rect.y,x[i+1]-x[i],rect.h),c);
		else
			SDL_FillRect(self->win,rp(rect.x,rect.y+y[i],rect.w,y[i+1]-y[i]),c);
	}
	rectangleColor(self->win,rect.x,rect.y,rect.x+rect.w-1,rect.y+rect.h-1,0x707070ff);
}

void WidgetsExtra::Color5::setup()
{
	if(not ready)
		this->set_col(this->c[0], this->c[4]);
}

const WidgetsExtra::Color5
	WidgetsExtra::Color5::GradientBlue(0xf0ffffff, 0xa0d0e0ff),
	WidgetsExtra::Color5::GradientDarkBlue(0xa0f0ffff, 0x3094d0ff),
	WidgetsExtra::Color5::GradientWheat(0xfff0d7ff, 0xc0c0a0ff),
	WidgetsExtra::Color5::GradientGrey(0xffffffff, 0xc7c7c7ff),
	WidgetsExtra::Color5::GradientDarkGrey(0xf0f0f0ff, 0x707070ff),
	WidgetsExtra::Color5::GradientGreen(0xe0ffe0ff, 0x77c077ff),
	WidgetsExtra::Color5::GradientRose(0xffd0d0ff, 0xd07070ff);



