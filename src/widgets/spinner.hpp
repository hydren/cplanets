/*
 * spinner.hpp
 *
 *  Created on: 12 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_SPINNER_HPP_
#define WIDGETS_SPINNER_HPP_

#include <stdexcept>

#include "SDL_widgets/SDL_widgets.h"
#include "futil/futil.hpp"

#include "abstract_layout.hpp"
#include "widgets_util.hpp"

namespace WidgetsExtra
{
	namespace Spinner_static
	{
		std::map<Button*, void*> references; //xxx kludge-type references. do not modify it!!!
		int nextId=0; //xxx Id sequence for Spinners. Do not modify it!!!
	}

	/** A widget like Java's JSpinner. It is a template, which means the value on the spinner field can be (theorectically) of any typename.
	 * Currently it works with 'int'-like typenames.
	 * The Type must have operator + and -, and be able to cast from double/float.*/
	template<typename Type>
	struct Spinner extends WidgetsExtra::Layout::Element
	{
		static const unsigned BUTTON_SIZE = 16;
		DialogWin spinner;
		Button btnInc, btnDec;

		Spinner(WinBase *pw, Rect area, const char* label=null, Type* value=null)
		: spinner(pw, Rect(area.x, area.y, area.w - BUTTON_SIZE, area.h)),
		  btnInc (pw, 0, Rect(area.x + area.w - BUTTON_SIZE, area.y, BUTTON_SIZE, BUTTON_SIZE/2), "+", changeValue),
		  btnDec (pw, 0, Rect(area.x + area.w - BUTTON_SIZE, area.y + BUTTON_SIZE/2, BUTTON_SIZE, BUTTON_SIZE/2), "-", changeValue),
		  value(value==null? new Type(1) : value), step(1), min(0), max(9999)
		{
			if(label != null) this->spinner.dialog_label(label);
			this->spinner.cmd = validateField;
			this->spinner.cmd_id = Spinner_static::nextId++;
			Spinner_static::references[&btnInc] = this; //register kludge-type reference to this spinner
			Spinner_static::references[&btnDec] = this; //register kludge-type reference to this spinner
		}

		virtual ~Spinner() {}

		virtual Point getPosition() const
		{
			return spinner.area;
		}

		virtual void setPosition(int x, int y)
		{
			WidgetsExtra::setComponentPosition(&spinner, x, y);
			WidgetsExtra::setComponentPosition(&btnInc, spinner.area.x + spinner.tw_area.w, spinner.area.y + TDIST);
			WidgetsExtra::setComponentPosition(&btnDec, spinner.area.x + spinner.tw_area.w, spinner.area.y + BUTTON_SIZE/2 + TDIST);
		}

		void setPosition(Point position)
		{
			setPosition(position.x, position.y);
		}

		virtual Rect getSize() const
		{
			return Rect(spinner.area.x, spinner.area.y, spinner.tw_area.w + btnInc.tw_area.w, spinner.tw_area.h);
		}

		virtual void setSize(Rect size)
		{
			spinner.widen(size.w - spinner.tw_area.w, size.h - spinner.tw_area.h);
			WidgetsExtra::setComponentPosition(&btnInc, spinner.area.x + spinner.tw_area.w, spinner.area.y + TDIST);
			WidgetsExtra::setComponentPosition(&btnDec, spinner.area.x + spinner.tw_area.w, spinner.area.y + BUTTON_SIZE/2 + TDIST);
		}

		virtual bool isStretched() const
		{
			return false; // todo maybe allow spinner to be layout-stretched (setSize() is ready, but untested)
		}

		virtual bool operator == (const Element& b) const
		{
			return &b == this;
		}

		Type* getValue()
		{
			return this->value;
		}

		void setValue(Type* val, bool deleteOldValue=false)
		{
			Type* old = this->value;

			this->value = val;
			if(sdl_running)
				refresh();

			if(deleteOldValue) delete old;
		}

		Type getStepValue()
		{
			return this->step;
		}

		void setStepValue(Type v)
		{
			if(v < 0) //step value must be non-negative
				throw std::invalid_argument("Specified step value in Spinner is negative");

			this->step = v;
		}

		void incrementValue()
		{
			if(isValidValue(*(this->value) + this->step) && *(this->value) + this->step >= *(this->value)) //second clausule checks for overflow
			{
				*(this->value) += this->step;
			}
			this->refresh();
		}

		void decrementValue()
		{
			if(isValidValue(*(this->value) - this->step) && *(this->value) - this->step <= *(this->value))
			{
				*(this->value) -= this->step;
			}
			this->refresh();
		}

		void setLabel(const char* lbl)
		{
			this->spinner.dialog_label(lbl);
		}

		/** Refresh the text according to this spinner's value.  */
		void refresh()
		{
			string strValue = string() + *(this->value);
			spinner.dialog_def(strValue.c_str(), this->spinner.cmd, this->spinner.cmd_id);
			spinner.unset_cursor();
			btnInc.draw_blit_upd();
			btnDec.draw_blit_upd();
		}

		protected:
		Type* value, step, min, max;

		bool isValidValue(Type val)
		{
			return (val >= this->min) && (val <= this->max);
		}

		static Spinner<Type>* getSpinner(Button* btn)
		{
			return static_cast<Spinner<Type>*> (Spinner_static::references[btn]); //kludged reference to the button's spinner
		}

		static Spinner<Type>* getSpinner(int kludgyId)
		{
			typedef std::pair<Button*, void*> PairOfButtonPtrAndVoidPtr;
			typedef std::map<Button*, void*> MapOfButtonPtrAndVoidPtr;
			const_foreach(const PairOfButtonPtrAndVoidPtr, pair, MapOfButtonPtrAndVoidPtr, Spinner_static::references)
			{
				Spinner<Type>* spinner = static_cast<Spinner<Type>*> (pair.second);
				if(spinner != null && spinner->spinner.cmd_id == kludgyId)
				{
					return spinner; //kludged reference to the button's spinner
				}
			}
			return null;
		}

		static void changeValue(Button* btn)
		{
			Spinner* sp = getSpinner(btn); //kludged reference to the button's spinner
			if(string(btn->label.str) == string("+")) //if '+' do increment
				sp->incrementValue();

			if(string(btn->label.str) == string("-")) //if '-' do decrement
				sp->decrementValue();
		}

		static void validateField(const char* text,int cmd_id)
		{
			Spinner* sp = getSpinner(cmd_id); //kludged reference to the button's spinner
			if(String::parseable<Type>(string(text))) //if we can figure out something from the field
			{
				Type val = String::parse<Type>(string(text));
				if(sp->isValidValue(val)) //if value type is inside bounds
					*sp->value = val; //change only the pointer
			}

			sp->refresh(); //do the real deal. if everything was alright, sp->getValue() is already the new value
		}
	};
}


#endif /* WIDGETS_SPINNER_HPP_ */
