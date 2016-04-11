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
	//stuff related to the main window
	namespace MainWindow
	{
		//show the main window and starts all its events treatments
		void show();
	}

	//convert the given sdl color to an int color suitable to be used on the given surface
	int colorToInt(const SDL_Surface* surf, const SDL_Color& color, bool forceRGBA=false);

	//set the color components of the given sdl color. use -1 to leave the component unchanged (i.e. modifyColor(-1,224,-1) only sets the green component to 224)
	void modifyColor(SDL_Color& color, int r, int g, int b);

	//creates a random color
	SDL_Color* getRandomColor();

	//trigger repainting of the whole window. it will be repainted eventually
	void triggerRepaint();

	//adjusts the given button size according to its label
	void packButton(Button* btn);

	void packCheckbox(CheckBox* chck);

	void setComponentPosition(WinBase* component, Point& position);
	void setComponentPosition(WinBase* component, int x, int y);
	void setComponentPositionX(WinBase* component, int x);
	void setComponentPositionY(WinBase* component, int y);
}

#endif /* MAIN_WINDOW_HPP_ */
