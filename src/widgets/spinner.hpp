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
		bool needsRefresh;

		Spinner(WinBase *pw, Rect area, const char* label=null, Type* value=null)
		: AbstractSpinner(pw, area, label),
		  value(value==null? new Type(1) : value), step(1), min(0), max(9999),
		  needsRefresh(false)
		{}

		/// Returns a pointer to the spinner current value.
		virtual Type* getValue()
		{
			return this->value;
		}

		/// Replaces the spinner value with the pointer 'val'. If 'deleteOldValue' is true, calls the previous value's destructor.
		virtual void setValue(Type* val, bool deleteOldValue=false)
		{
			Type* old = this->value;

			this->value = val;
			this->needsRefresh = true;

			if(deleteOldValue) delete old;
		}

		/// Updates the spinner value, assigning 'newValue' to it.
		virtual void setValue(const Type& newValue)
		{
			*(this->value) = newValue;
			this->needsRefresh = true;
		}

		/// Returns the value by which the spinner's value is incremented or decremented when calling incrementValue() or decrementValue()
		virtual Type getStepValue()
		{
			return this->step;
		}

		/// Sets the value by which the spinner's value is incremented or decremented to 'v'. 'v' must be non-negative.
		virtual void setStepValue(Type v)
		{
			if(v < 0) //step value must be non-negative
				throw std::invalid_argument("Specified step value in Spinner is negative");

			this->step = v;
		}

		/// Return true if 'val' is in the valid range of this spinner's value.
		virtual bool isValidValue(Type val)
		{
			return (val >= this->min) && (val <= this->max);
		}

		/// Updates the text field content and redraw's the Spinner.
		virtual void refresh()
		{
			string strValue = string() + *(this->value);
			this->dlwTextField.dialog_def(strValue.c_str(), this->dlwTextField.cmd, this->dlwTextField.cmd_id);
			this->dlwTextField.unset_cursor();
			this->btnInc.draw_blit_upd();
			this->btnDec.draw_blit_upd();
			this->needsRefresh = false;
		}

		//overrides AbstractSpinner's
		virtual void draw()
		{
			if(this->needsRefresh)
				this->refresh();
			else
				this->AbstractSpinner::draw();
		}

		/// Increments the spinner value by 'step'.
		virtual void incrementValue()
		{
			if(isValidValue(*(this->value) + this->step) && *(this->value) + this->step >= *(this->value)) //second clausule checks for overflow
			{
				*(this->value) += this->step;
			}
			this->refresh();
		}

		/// Decrements the spinner value by 'step'.
		virtual void decrementValue()
		{
			if(isValidValue(*(this->value) - this->step) && *(this->value) - this->step <= *(this->value))
			{
				*(this->value) -= this->step;
			}
			this->refresh();
		}

		//overrides AbstractSpinner's
		virtual void parseIfValid(const char* txtVal)
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
