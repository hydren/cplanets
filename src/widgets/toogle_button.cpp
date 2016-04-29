/*
 * toogle_button.cpp
 *
 *  Created on: 29 de abr de 2016
 *      Author: felipe
 */

#include "toogle_button.hpp"

using SDL_util::ToogleButton;

// copied from SDL_widgets.cpp (I know, soooo kludged)
struct Color5 {
  Uint32 c[5];
  void set_col(Uint32 fst,Uint32 lst);  // input like: 0x102030ff
};

// copied from SDL_widgets.cpp (I know, soooo kludged)
static Color5
  cGradientBlue, cGradientDarkBlue, cGradientRose, cGradientWheat, cGradientGreen,
  cGradientGrey, cGradientDarkGrey;

ToogleButton::ToogleButton(WinBase *pw,Style st,Rect rect,Label lab,void (*_cmd)(CheckBox*),Id _id)
: CheckBox(pw, st, rect, lab, _cmd, _id)
{
	// copied from SDL_widgets.cpp (I know, soooo kludged)
	cGradientBlue.set_col    (0xf0ffffff,0xa0d0e0ff);
	cGradientDarkBlue.set_col(0xa0f0ffff,0x3094d0ff);
	cGradientWheat.set_col   (0xfff0d7ff,0xc0c0a0ff);
	cGradientGrey.set_col    (0xffffffff,0xc7c7c7ff);
	cGradientDarkGrey.set_col(0xf0f0f0ff,0x707070ff);
	cGradientGreen.set_col   (0xe0ffe0ff,0x77c077ff);
	cGradientRose.set_col    (0xffd0d0ff,0xd07070ff);
}

void ToogleButton::draw()
{
	if (!win)
		win=SDL_CreateRGBSurface(SDL_SWSURFACE,tw_area.w,tw_area.h,16,0,0,0,0);

	// copied from SDL_widgets.cpp (I know, soooo kludged)
	static const Color5 *grad_colors[5]={
			&cGradientBlue,&cGradientGrey,&cGradientGreen,&cGradientWheat,&cGradientRose
	};
	const Color5 *bcol=(style.param<5 && style.param>0 ? grad_colors[style.param] : grad_colors[0]);

	int y=tw_area.h/2-8;

	if (*d)
		draw_raised(0,bcol->c[2],false);
	else
		draw_gradient(Rect(0,0,tw_area.w,tw_area.h),bcol);
	label.draw(win,id.id2,Point(3,y));
}
