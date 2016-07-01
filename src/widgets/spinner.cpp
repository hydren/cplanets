/*
 * spinner.cpp
 *
 *  Created on: 24 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "spinner.hpp"
#include <map>

#include "widgets_util.hpp"

using WidgetsExtra::AbstractSpinner;
using WidgetsExtra::Spinner;

// helpers
namespace AbstractSpinnerAux
{
	int nextId = 0; // used to maintain consistency. not thread safe.
	std::map<int, AbstractSpinner*> references; //xxx kludge to map references to Spinner's addresses by id
}

AbstractSpinner::AbstractSpinner(WinBase *pw, Rect area, const char* label)
: WinBase(pw, null, area.x, area.y, area.w, area.h, pw->bgcol, ++AbstractSpinnerAux::nextId),
  dlwTextField(this, Rect(0, 0, area.w - BUTTON_SIZE, area.h)),
  btnInc (this, 0, Rect(area.w - BUTTON_SIZE, TDIST, BUTTON_SIZE, BUTTON_SIZE/2), "+", changeValue),
  btnDec (this, 0, Rect(area.w - BUTTON_SIZE, BUTTON_SIZE/2 + TDIST, BUTTON_SIZE, BUTTON_SIZE/2), "-", changeValue)
{
	if(label != null) this->dlwTextField.dialog_label(label);
	this->dlwTextField.cmd = validateField;
	this->dlwTextField.cmd_id = this->id.id1;
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
	dlwTextField.widen(dx, dy);
	WidgetsExtra::setComponentPosition(&btnInc, dlwTextField.tw_area.w, TDIST);
	WidgetsExtra::setComponentPosition(&btnDec, dlwTextField.tw_area.w, BUTTON_SIZE/2 + TDIST);
}

void AbstractSpinner::setLabel(const char* lbl)
{
	this->dlwTextField.dialog_label(lbl);
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
	sp->assignValue(text);
	sp->refresh(); //do the real deal. if everything was alright, sp->getValue() is already the new value
}
