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
struct Spinner : DialogWin
{
	static const unsigned REFERENCE_SIZE = 32;
	Button btnInc, btnDec;

	Spinner(WinBase *pw,Rect area,Id id=0)
	: DialogWin(pw, Rect(area.x, area.y, area.w - REFERENCE_SIZE, area.h), id),
	  btnInc(pw, 0, Rect(area.x + area.w, area.y                 , REFERENCE_SIZE/2, REFERENCE_SIZE/4), "+", null),
	  btnDec(pw, 0, Rect(area.x + area.w, area.y + REFERENCE_SIZE/4, REFERENCE_SIZE/2, REFERENCE_SIZE/4), "-", null),
	  value()
	{
		this->add_child(&btnInc);
		this->add_child(&btnDec);
	}

	virtual ~Spinner() {}

	void draw()
	{
		init_gui();

		DialogWin::draw();
	}

	Type getValue()
	{
		return this->value;
	}

	void setValue(Type val)
	{
		this->value = val;
		string strValue = string()+val;
		dialog_def(strValue.c_str(), this->cmd, this->cmd_id);
	}

	void updateButtonPosition()
	{
		CPlanetsGUI::setComponentPosition(&btnInc, this->area.x + this->tw_area.w, this->area.y + TDIST);
		CPlanetsGUI::setComponentPosition(&btnDec, this->area.x + this->tw_area.w, this->area.y + REFERENCE_SIZE/4 + TDIST);
	}

	void widen(int dx=0,int dy=0)
	{
		if(dx != 0 || dy != 0)
			DialogWin::widen(dx, dy);

		updateButtonPosition();
	}

	private:
	Type value;
};

#endif /* SPINNER_HPP_ */
