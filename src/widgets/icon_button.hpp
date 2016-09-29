/*
 * icon_button.hpp
 *
 *  Created on: 29 de set de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_ICON_BUTTON_HPP_
#define WIDGETS_ICON_BUTTON_HPP_
#include <ciso646>

#include "SDL_widgets/SDL_widgets.h"
#include "futil/general/language.hpp"

namespace WidgetsExtra
{
	struct IconButton extends Button
	{
		// pass one of this modes to style.param2
		enum IconMode { ICON_CENTER, ICON_LEFT, ICON_RIGHT, ICON_CENTER_UP, ICON_CENTER_DOWN, ICON_LEFT_UP, ICON_LEFT_DOWN, ICON_RIGHT_UP, ICON_RIGHT_DOWN };

		SDL_Surface* icon;
		IconButton(WinBase *parent, Style style, Rect bounds, Label label, SDL_Surface* icon, void (*cmd)(Button*), Id id=0);
		virtual ~IconButton();
		virtual void draw();
	};
}

#endif /* WIDGETS_ICON_BUTTON_HPP_ */
