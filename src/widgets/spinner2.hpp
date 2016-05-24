/*
 * spinner.hpp
 *
 *  Created on: 24 de mai de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_SPINNER2_HPP_
#define WIDGETS_SPINNER2_HPP_

#include "SDL_widgets/SDL_widgets.h"
#include "futil/futil.hpp"
#include "abstract_layout.hpp"

namespace WidgetsExtra
{
	struct AbstractSpinner extends WinBase, WidgetsExtra::Layout::WinBaseWrapper
	{
		static const unsigned BUTTON_SIZE = 16;

		DialogWin spinner;
		Button btnInc, btnDec;

		AbstractSpinner(WinBase *pw, Rect area, const char* label=null);
		virtual ~AbstractSpinner();

		virtual void draw();

		virtual Point getPosition() const;

		virtual void setPosition(int x, int y);

		void setPosition(Point position);

		virtual Rect getSize() const;

		virtual void setSize(Rect size);

		virtual bool isStretched() const;

		virtual bool operator == (const Element& b) const;

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
		static void changeValue(Button* btn);
		static void validateField(const char* text,int cmd_id);
	};

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
			spinner.dialog_def(strValue.c_str(), this->spinner.cmd, this->spinner.cmd_id);
			spinner.unset_cursor();
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



#endif /* WIDGETS_SPINNER2_HPP_ */
