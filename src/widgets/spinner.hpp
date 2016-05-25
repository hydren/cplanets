/*
 * spinner.hpp
 *
 *  Created on: 12 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_SPINNER_HPP_
#define WIDGETS_SPINNER_HPP_

#include "SDL_widgets/SDL_widgets.h"

#include <stdexcept>

#include "futil/futil.hpp"
#include "abstract_layout.hpp"

namespace WidgetsExtra
{
	/// Generic widget that serves as a skeleton to the template struct Spinner.
	struct AbstractSpinner extends WinBase
	{
		static const unsigned BUTTON_SIZE = 16;

		DialogWin dlwTextField;
		Button btnInc, btnDec;

		AbstractSpinner(WinBase *pw, Rect area, const char* label=null);
		virtual ~AbstractSpinner();

		//overrides WinBase's
		virtual void draw();

		//overrides WinBase's (caution: doesn't work if called from a WinBase pointer; widen() is not virtual in WinBase)
		virtual void widen(int dx, int dy);

		/// Sets the spinner label
		virtual void setLabel(const char* lbl);

		//pure virtual. needs to be implemented:

		/// Should refresh the text according to this spinner's current value.
		virtual void refresh() abstract;

		/// Should increment the spinner value
		virtual void incrementValue() abstract;

		/// Should decrement the spinner value
		virtual void decrementValue() abstract;

		/// Should, if the text in the field is valid, update the spinner value. Otherwise refresh the field text with the spinner value (overriding any text in the field)
		virtual void parseIfValid(const char* txtVal) abstract;

		protected:
		//callbacks
		static void changeValue(Button* btn);
		static void validateField(const char* text,int cmd_id);
	};

	/** A widget similar to Swing's JSpinner. It is a template, which means the value on the spinner field can be (theorectically) of any type.
	 * Currently it has been tested (and passed) with integer types (int, long and alike) and floating points types (float, double and alike).
	 * The template type must have operator + and -, and be able to cast from literal values 1 and 9999.
	 * Default bounds are 0 (min) and 9999 (max). Set these as needed. Default step value is literal value 1.*/
	template<typename Type>
	struct Spinner extends AbstractSpinner
	{
		Type* value, step, min, max;

		Spinner(WinBase *pw, Rect area, const char* label=null, Type* value=null)
		: AbstractSpinner(pw, area, label),
		  value(value==null? new Type(1) : value), step(1), min(0), max(9999)
		{}

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

		bool isValidValue(Type val)
		{
			return (val >= this->min) && (val <= this->max);
		}

		void refresh()
		{
			string strValue = string() + *(this->value);
			dlwTextField.dialog_def(strValue.c_str(), this->dlwTextField.cmd, this->dlwTextField.cmd_id);
			dlwTextField.unset_cursor();
			btnInc.draw_blit_upd();
			btnDec.draw_blit_upd();
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

		void parseIfValid(const char* txtVal)
		{
			if(String::parseable<Type>(string(txtVal))) //if we can figure out something from the field
			{
				Type val = String::parse<Type>(string(txtVal));
				if(this->isValidValue(val)) //if value type is inside bounds
					*(this->value) = val;
			}
		}
	};
}

#endif /* WIDGETS_SPINNER_HPP_ */
