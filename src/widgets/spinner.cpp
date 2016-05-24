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

namespace AbstractSpinnerDetails
{
	int nextId = 0;
	std::map<int, AbstractSpinner*> references; //xxx kludge-type to reference Spinner's by id
}

AbstractSpinner::AbstractSpinner(WinBase *pw, Rect area, const char* label)
: WinBase(pw, null, area.x, area.y, area.w, area.h, pw->bgcol, ++AbstractSpinnerDetails::nextId),
  WinBaseWrapper(this),
  spinner(this, Rect(area.x, area.y, area.w - BUTTON_SIZE, area.h)),
  btnInc (this, 0, Rect(area.x + area.w - BUTTON_SIZE, area.y, BUTTON_SIZE, BUTTON_SIZE/2), "+", changeValue),
  btnDec (this, 0, Rect(area.x + area.w - BUTTON_SIZE, area.y + BUTTON_SIZE/2, BUTTON_SIZE, BUTTON_SIZE/2), "-", changeValue)
{
	if(label != null) this->spinner.dialog_label(label);
	this->spinner.cmd = validateField;
	this->spinner.cmd_id = this->id.id1;
	AbstractSpinnerDetails::references[this->id.id1] = this; //register kludge-type reference to this spinner
}

AbstractSpinner::~AbstractSpinner()
{}

void AbstractSpinner::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, this->bgcol); //clears the screen
}

Point AbstractSpinner::getPosition() const
{
	return spinner.area;
}

void AbstractSpinner::setPosition(int x, int y)
{
	WidgetsExtra::setComponentPosition(&spinner, x, y);
	WidgetsExtra::setComponentPosition(&btnInc, spinner.area.x + spinner.tw_area.w, spinner.area.y + TDIST);
	WidgetsExtra::setComponentPosition(&btnDec, spinner.area.x + spinner.tw_area.w, spinner.area.y + BUTTON_SIZE/2 + TDIST);
}

void AbstractSpinner::setPosition(Point position)
{
	setPosition(position.x, position.y);
}

Rect AbstractSpinner::getSize() const
{
	return Rect(spinner.area.x, spinner.area.y, spinner.tw_area.w + btnInc.tw_area.w, spinner.tw_area.h);
}

void AbstractSpinner::setSize(Rect size)
{
	spinner.widen(size.w - spinner.tw_area.w, size.h - spinner.tw_area.h);
	WidgetsExtra::setComponentPosition(&btnInc, spinner.area.x + spinner.tw_area.w, spinner.area.y + TDIST);
	WidgetsExtra::setComponentPosition(&btnDec, spinner.area.x + spinner.tw_area.w, spinner.area.y + BUTTON_SIZE/2 + TDIST);
}

bool AbstractSpinner::isStretched() const
{
	return false; // todo maybe allow spinner to be layout-stretched (setSize() is ready, but untested)
}

bool AbstractSpinner::operator == (const Element& b) const
{
	return &b == this;
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
	AbstractSpinner* sp = AbstractSpinnerDetails::references[cmd_id]; //kludged reference to the button's spinner
	sp->parseIfValid(text);
	sp->refresh(); //do the real deal. if everything was alright, sp->getValue() is already the new value
}
