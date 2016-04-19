/*
 * label_win.hpp
 *
 *  Created on: 19 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_LABEL_WIN_HPP_
#define WIDGETS_LABEL_WIN_HPP_

#include <string>

#include "SDL_widgets/SDL_widgets.h"

#include "futil/futil.hpp"


struct LabelWin extends WinBase
{
	std::string txt;

	//WinBase *pw,const char *t,int x,int y,int dx,int dy,Uint32 bgcol,Id id
	LabelWin(WinBase *pw, Rect bounds, std::string str, Id id=0);

	virtual ~LabelWin();
	virtual void draw();
};



#endif /* WIDGETS_LABEL_WIN_HPP_ */
