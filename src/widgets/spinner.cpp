/*
 * spinner.cpp
 *
 *  Created on: 24 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "spinner2.hpp"
#include <map>

#include "widgets_util.hpp"

using WidgetsExtra::AbstractSpinner;
using WidgetsExtra::Spinner;

namespace AbstractSpinnerAux
{
	int nextId = 0;
	std::map<int, AbstractSpinner*> references; //xxx kludge-type to reference Spinner's by id
}

AbstractSpinner::AbstractSpinner(WinBase *pw, Rect area, const char* label)
: WinBase(pw, null, area.x, area.y, area.w, area.h, pw->bgcol, ++AbstractSpinnerAux::nextId),
  spinner(this, Rect(0, 0, area.w - BUTTON_SIZE, area.h)),
  btnInc (this, 0, Rect(area.w - BUTTON_SIZE, TDIST, BUTTON_SIZE, BUTTON_SIZE/2), "+", changeValue),
  btnDec (this, 0, Rect(area.w - BUTTON_SIZE, BUTTON_SIZE/2 + TDIST, BUTTON_SIZE, BUTTON_SIZE/2), "-", changeValue)
{
	if(label != null) this->spinner.dialog_label(label);
	this->spinner.cmd = validateField;
	this->spinner.cmd_id = this->id.id1;
	AbstractSpinnerAux::references[this->id.id1] = this; //register kludge-type reference to this spinner
}

AbstractSpinner::~AbstractSpinner()
{}

void AbstractSpinner::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, this->bgcol); //clears the screen
}

void AbstractSpinner::widen(int dx, int dy)
{
	spinner.widen(dx, dy);
	WidgetsExtra::setComponentPosition(&btnInc, spinner.tw_area.w, TDIST);
	WidgetsExtra::setComponentPosition(&btnDec, spinner.tw_area.w, BUTTON_SIZE/2 + TDIST);
}

void AbstractSpinner::setLabel(const char* lbl)
{
	this->spinner.dialog_label(lbl);
}


// static methods

void AbstractSpinner::changeValue(Button* btn)
{
	AbstractSpinner* sp = static_cast<AbstractSpinner*>(btn->parent);
	if(string(btn->label.str) == string("+")) //if '+' do increment
		sp->incrementValue();

	if(string(btn->label.str) == string("-")) //if '-' do decrement
		sp->decrementValue();
}

void AbstractSpinner::validateField(const char* text,int cmd_id)
{
	AbstractSpinner* sp = AbstractSpinnerAux::references[cmd_id]; //kludged reference to the button's spinner
	sp->parseIfValid(text);
	sp->refresh(); //do the real deal. if everything was alright, sp->getValue() is already the new value
}
