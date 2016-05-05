/*
 * SDL_utils.hpp
 *
 *  Created on: 13 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef SDL_UTIL_HPP_
#define SDL_UTIL_HPP_

//SDL-related utilities

#include <SDL/SDL.h>

#include "hotfixes.h" //macros to enable/disable hotfixes for SDL-related issues

//works kinda like java's synchronized block
#define synchronized(mutex) for(bool mutex##_SYNCHRONIZED=SDL_mutexP(mutex)||true;mutex##_SYNCHRONIZED;mutex##_SYNCHRONIZED=SDL_mutexV(mutex)&&false)

namespace SDL_util
{
	namespace Color
	{
		const SDL_Color
			BLACK 	= {  0,  0,  0,0},
			RED   	= {255,  0,  0,0},
			GREEN 	= {  0,255,  0,0},
			BLUE  	= {  0,  0,255,0},
			YELLOW	= {255,255,  0,0},
			CYAN 	= {  0,255,255,0},
			MAGENTA	= {255,  0,255,0},
			WHITE 	= {255,255,255,0};
	}

	//convert the given sdl color to an int color suitable to be used on the given surface
	int colorToInt(const SDL_Surface* surf, const SDL_Color& color, bool forceRGBA=false);

	//creates a random color
	SDL_Color* getRandomColor();

	void setWindowTitle(const char* title);
	void setWindowIcon(const char* icon);
}

#endif /* SDL_UTIL_HPP_ */
