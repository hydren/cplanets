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

//works kinda like java's synchronized block
#define synchronized(mutex) for(bool mutex##_SYNCHRONIZED=SDL_mutexP(mutex)||true;mutex##_SYNCHRONIZED;mutex##_SYNCHRONIZED=SDL_mutexV(mutex)&&false)

namespace SDL_util
{
	namespace Color
	{
		const SDL_Color BLACK = {0,0,0,0};

		//full saturated colors
		const SDL_Color
			RED   	= {255,  0,  0,  0},
			LIME 	= {  0,255,  0,  0},
			BLUE  	= {  0,  0,255,  0},
			YELLOW	= {255,255,  0,  0},
			CYAN 	= {  0,255,255,  0},
			MAGENTA	= {255,  0,255,  0},
			WHITE 	= {255,255,255,  0};

		//half saturated colors
		const SDL_Color
			MAROON 	= {127,  0,  0,  0},
			GREEN 	= {  0,127,  0,  0},
			NAVY  	= {  0,  0,127,  0},
			OLIVE	= {127,127,  0,  0},
			TEAL 	= {  0,127,127,  0},
			PURPLE	= {127,  0,127,  0},
			GRAY 	= {127,127,127,  0};

		//full/half saturated colors
		const SDL_Color
			ORANGE			= {255,127,  0,  0},
			SPRING_GREEN 	= {  0,255,127,  0},
			ROSE			= {255,  0,127,  0},
			CHARTREUSE		= {127,255,  0,  0},
			AZURE 			= {  0,127,255,  0},
			VIOLET			= {127,  0,255,  0};
	}

	//convert the given sdl color to an int color suitable to be used on the given surface
	int colorToInt(const SDL_Surface* surf, const SDL_Color& color, bool forceRGBA=false);

	//creates a random color
	SDL_Color* getRandomColor();

	SDL_Surface* loadBitmap(const char* path, const SDL_Color* transparentColor=NULL);

	//hints SDL_Init to start the window centered
	void preloadCentered();
}

#endif /* SDL_UTIL_HPP_ */
