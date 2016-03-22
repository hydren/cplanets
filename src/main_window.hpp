/*
 * main_window.hpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#ifndef MAIN_WINDOW_HPP_
#define MAIN_WINDOW_HPP_

#include "sdl-widgets/sdl-widgets.h"

namespace CPlanetsGUI
{
	namespace MainWindow
	{
		void show();
	}

	int colorToInt(const SDL_Surface* surf, const SDL_Color& color, bool forceRGBA=false);
	void modifyColor(SDL_Color& color, int r, int g, int b);
	SDL_Color* getRandomColor();
}

#endif /* MAIN_WINDOW_HPP_ */
