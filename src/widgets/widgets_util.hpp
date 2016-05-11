/*
 * widgets_util.hpp
 *
 *  Created on: 27 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_WIDGETS_UTIL_HPP_
#define WIDGETS_WIDGETS_UTIL_HPP_

#include "SDL_widgets/SDL_widgets.h"

///Utilities for SDL_widgets
namespace WidgetsExtra
{
	/// Properly sets the position of the given component (better than modifying .area and .tw_area directly).
	void setComponentPosition(WinBase* component, const Point& position);

	/// Properly sets the position of the given component (better than modifying .area and .tw_area directly).
	void setComponentPosition(WinBase* component, int x, int y);

	/// Properly sets the position of the given component (better than modifying .area and .tw_area directly).
	void setComponentPositionX(WinBase* component, int x);

	/// Properly sets the position of the given component (better than modifying .area and .tw_area directly).
	void setComponentPositionY(WinBase* component, int y);

	/// Adjusts the given button size according to its label
	void packLabeledComponent(Button* btn, unsigned height=0, int padding=4);

	/// Adjusts the given checkbox size according to its label
	void packLabeledComponent(CheckBox* chck, unsigned height=0, int padding=4);

	/// Adjusts the given external radio button size according to its label
	void packLabeledComponent(RExtButton* rebt, unsigned height=0, int padding=4);

	/// Adjusts the given component size according to the given label
	void packLabeledComponent(WinBase* wb, Label& lbl, unsigned height=0, int padding=4);

	/// Helper class to hold parameters. Purely optional.
	struct LabeledComponentPacker
	{
		unsigned height; int padding;
		LabeledComponentPacker(unsigned height=0, int padding=4) : height(height), padding(padding) {}
		void pack(Button* btn) const { packLabeledComponent(btn, height, padding); }
		void pack(CheckBox* chck) const { packLabeledComponent(chck, height, padding); }
		void pack(RExtButton* rebt) const { packLabeledComponent(rebt, height, padding); }
		void pack(WinBase* wb, Label& lbl) const { packLabeledComponent(wb, lbl, height, padding); }
	};

	/// A (proposed) default draw() function for BgrWin's. Fills its area with background color.
	void drawDefaultBgrWin(BgrWin* bgrWin);

	/// Holds a gradient of 5 colors (copied from SDL_widgets.cpp)
	struct Color5 {
		Uint32 c[5];
		Color5(Uint32 fst,Uint32 lst);
		void set_col(Uint32 fst,Uint32 lst);  // input like: 0x102030ff
		void draw_gradient(WinBase* wb,Rect rect,bool vertical=false,bool hollow=false) const;

		static const Color5
			GradientBlue, GradientDarkBlue, GradientRose, GradientWheat, GradientGreen,
			GradientGrey, GradientDarkGrey;

		protected:
		bool ready;
		void setup();
	};


}

#endif /* WIDGETS_WIDGETS_UTIL_HPP_ */
