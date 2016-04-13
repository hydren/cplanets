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
namespace CPlanetsGUI
{
	const int USER_EVENT_ID__REDRAW_COMPONENT = 192;

	//adjusts the given button size according to its label
	void packLabeledComponent(Button* btn);

	//adjusts the given checkbox size according to its label
	void packLabeledComponent(CheckBox* chck);

	void setComponentPosition(WinBase* component, Point& position);
	void setComponentPosition(WinBase* component, int x, int y);
	void setComponentPositionX(WinBase* component, int x);
	void setComponentPositionY(WinBase* component, int y);

	//stuff related to the main window
	namespace MainWindow
	{
		//show the main window and starts all its events treatments
		void show();
	}
}

#endif /* MAIN_WINDOW_HPP_ */
