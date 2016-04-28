/*
 * main_window.hpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#ifndef MAIN_WINDOW_HPP_
#define MAIN_WINDOW_HPP_

#include <string>

#include <SDL_widgets/SDL_widgets.h>

#include "util.hpp"

extern const unsigned WIDGETS_SPACING;

//contains all cplanets gui related stuff
namespace CPlanets
{
	const int USER_EVENT_ID__REDRAW_COMPONENT = 192;

	//show the main window and starts all its events treatments
	void showMainWindow();
}

#endif /* MAIN_WINDOW_HPP_ */
