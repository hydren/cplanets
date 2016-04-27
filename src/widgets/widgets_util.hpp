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
namespace SDL_util
{
	void setComponentPosition(WinBase* component, Point& position);
	void setComponentPosition(WinBase* component, int x, int y);
	void setComponentPositionX(WinBase* component, int x);
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
		void pack(Button* btn) { packLabeledComponent(btn, height, padding); }
		void pack(CheckBox* chck) { packLabeledComponent(chck, height, padding); }
		void pack(RExtButton* rebt) { packLabeledComponent(rebt, height, padding); }
		void pack(WinBase* wb, Label& lbl) { packLabeledComponent(wb, lbl, height, padding); }
	};
}

#endif /* WIDGETS_WIDGETS_UTIL_HPP_ */
