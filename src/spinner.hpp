/*
 * spinner.hpp
 *
 *  Created on: 12 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef SPINNER_HPP_
#define SPINNER_HPP_

#include "SDL_widgets/SDL_widgets.h"
#include "futil/futil.hpp"

template<typename Type>
struct Spinner : WinBase
{
	static const unsigned BUTTON_SIZE = 16;
	DialogWin spinner;
	Button btnInc, btnDec;

	Spinner(WinBase *pw,Rect area,const char* txt,Id id=0)
	: WinBase(pw, null, area.x, area.y, area.w, area.h, 0, id),
	  spinner(pw, Rect(area.x, area.y, area.w - BUTTON_SIZE, area.h), 0),
	  btnInc (pw, 0, Rect(area.x + area.w - BUTTON_SIZE, area.y, BUTTON_SIZE, BUTTON_SIZE/2), "+", changeValue),
	  btnDec (pw, 0, Rect(area.x + area.w - BUTTON_SIZE, area.y + BUTTON_SIZE/2, BUTTON_SIZE, BUTTON_SIZE/2), "-", changeValue),
	  value()
	{
		this->add_child(&spinner);
		this->add_child(&btnInc);
		this->add_child(&btnDec);
		this->spinner.dialog_label(txt);
	}

	virtual ~Spinner() {}

	void draw()
	{
		init_gui();
		SDL_FillRect(win,0,parent->bgcol);
	}

	Type getValue()
	{
		return this->value;
	}

	void setValue(Type val)
	{
		this->value = val;
		string strValue = string()+val;
		spinner.dialog_def(strValue.c_str(), this->spinner.cmd, this->spinner.cmd_id);
	}

	void validate()
	{
		CPlanetsGUI::setComponentPosition(&spinner,this->area.x, this->area.y);
		CPlanetsGUI::setComponentPosition(&btnInc, this->area.x + this->tw_area.w - BUTTON_SIZE, this->area.y + TDIST);
		CPlanetsGUI::setComponentPosition(&btnDec, this->area.x + this->tw_area.w - BUTTON_SIZE, this->area.y + BUTTON_SIZE/2 + TDIST);
	}

	void widen(int dx=0,int dy=0)
	{
		WinBase::widen(dx, dy);
		validate();
	}

	private:
	static void changeValue(Button*)
	{
		//fixme need to deference the Spinner... how?
	}
	Type value;
};

#endif /* SPINNER_HPP_ */
