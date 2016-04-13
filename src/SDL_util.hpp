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
	//convert the given sdl color to an int color suitable to be used on the given surface
	int colorToInt(const SDL_Surface* surf, const SDL_Color& color, bool forceRGBA=false);

	//set the color components of the given sdl color. use -1 to leave the component unchanged (i.e. modifyColor(-1,224,-1) only sets the green component to 224)
	void modifyColor(SDL_Color& color, int r, int g, int b);

	//creates a random color
	SDL_Color* getRandomColor();
}

#endif /* SDL_UTIL_HPP_ */
