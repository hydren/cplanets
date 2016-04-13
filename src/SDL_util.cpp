/*
 * SDL_utils.cpp
 *
 *  Created on: 13 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "SDL_util.hpp"

#include "futil/futil.hpp"

int SDL_util::colorToInt(const SDL_Surface* surf, const SDL_Color& color, bool forceRGBA)
{
	#ifdef HOTFIX_FOR_SDL_MAP_RGB_1
	if(forceRGBA || surf == null)
		return (((int) color.r) << 24) + (((int) color.g) << 16) + (((int) color.b) << 8) + 0x000000FF;

	#endif

	return SDL_MapRGB(surf->format, color.r, color.g, color.b);
}

void SDL_util::modifyColor(SDL_Color& color, int r, int g, int b)
{
	if(r != -1) color.r = r;
	if(b != -1) color.b = b;
	if(g != -1) color.g = g;
}

SDL_Color* SDL_util::getRandomColor()
{
	SDL_Color* somecolor = new SDL_Color;
	somecolor->r = Math::randomBetween(0, 255);
	somecolor->g = Math::randomBetween(0, 255);
	somecolor->b = Math::randomBetween(0, 255);
	return somecolor;
}
