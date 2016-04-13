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
struct Spinner;

namespace SpinnerUtil
{
	std::map<Button*, void*> references; //xxx kludge-type references. do not modify it!!!
	template <typename Type> Spinner<Type>* getSpinner(Button* btn)
	{
		return (Spinner<Type>*) references[btn]; //kludged reference to the button's spinner
	}
	template <typename Type> Spinner<Type>* getSpinner(int kludgyId)
	{
		typedef std::pair<Button*, void*> PairOfButtonPtrAndVoidPtr;
		typedef std::map<Button*, void*> MapOfButtonPtrAndVoidPtr;
		const_foreach(const PairOfButtonPtrAndVoidPtr, pair, MapOfButtonPtrAndVoidPtr, references)
		{
			Spinner<Type>* spinner = (Spinner<Type>*) pair.second;
			if(spinner != null && spinner->spinner.cmd_id == kludgyId)
			{
				return spinner;
			}
		}
		return null;
	}

	int nextId=0; //xxx kludge-type references. do not modify it!!!
}

/** A widget like Java's JSpinner. It is a template, which means the value on the spinner field can be (theorectically) of any typename.
 * Currently it works with 'int'-like typenames.
 * The Type must have operator + and -, and be able to cast from double/float.*/
template<typename Type>
struct Spinner : WinBase
{
	static const unsigned BUTTON_SIZE = 16;
	DialogWin spinner;
	Button btnInc, btnDec;

	Spinner(WinBase *pw,Rect area,const char* txt,Id id=0)
	: WinBase(pw, null, area.x, area.y, area.w, area.h, 0, id),
	  spinner(pw, Rect(area.x, area.y, area.w - BUTTON_SIZE, area.h), ++SpinnerUtil::nextId),
	  btnInc (pw, 0, Rect(area.x + area.w - BUTTON_SIZE, area.y, BUTTON_SIZE, BUTTON_SIZE/2), "+", changeValue),
	  btnDec (pw, 0, Rect(area.x + area.w - BUTTON_SIZE, area.y + BUTTON_SIZE/2, BUTTON_SIZE, BUTTON_SIZE/2), "-", changeValue),
	  value(new Type(1)), step(1), min(0), max(9999)
	{
		this->add_child(&spinner);
		this->add_child(&btnInc);
		this->add_child(&btnDec);
		this->spinner.dialog_label(txt);
		this->spinner.cmd = validateField;
		SpinnerUtil::references[&btnInc] = this; //register kludge-type reference to this spinner
		SpinnerUtil::references[&btnDec] = this; //register kludge-type reference to this spinner
	}

	virtual ~Spinner() {}

	void draw()
	{
		init_gui();
		SDL_FillRect(win,0,parent->bgcol);
	}

	Type* getValue()
	{
		return this->value;
	}

	void setValue(Type* val)
	{
		this->value = val;
		string strValue = string()+*val;
		spinner.dialog_def(strValue.c_str(), this->spinner.cmd, this->spinner.cmd_id);
	}

	Type getStepValue()
	{
		return this->step;
	}

	void setStepValue(Type v=1.0)
	{
		this->step = v;
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
	Type* value, step, min, max;

	static void changeValue(Button* btn)
	{
		Spinner* sp = SpinnerUtil::getSpinner<Type>(btn); //kludged reference to the button's spinner
		if(string(btn->label.str) == string("+") //if + do increment
			 && *(sp->value) + sp->step < sp->max //user bounds check
			 && *(sp->value) + sp->step > *(sp->value)) //overflow check
			*(sp->value) += sp->step;

		//else do decrement
		else if(*(sp->value) - sp->step > sp->min //user bounds check
			 && *(sp->value) - sp->step < *(sp->value)) //overflow check
			*(sp->value) -= sp->step;

		sp->setValue(sp->getValue());
	}

	static void validateField(const char* text,int cmd_id)
	{
		Spinner* sp = SpinnerUtil::getSpinner<Type>(cmd_id); //kludged reference to the button's spinner
		if(String::parseable<Type>(string(text)))
		{
			sp->value = new Type(String::parse<Type>(string(text)));
		}
		else sp->setValue(sp->getValue());
	}
};

#endif /* SPINNER_HPP_ */
