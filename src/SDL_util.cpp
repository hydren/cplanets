/*
 * SDL_utils.cpp
 *
 *  Created on: 13 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "SDL_util.hpp"

#include "hotfixes.h" //macros to enable/disable hotfixes for SDL-related issues

#include "futil/futil.hpp"

int SDL_util::colorToInt(const SDL_Surface* surf, const SDL_Color& color, bool forceRGBA)
{
	#ifdef HOTFIX_FOR_SDL_MAP_RGB_1
	if(forceRGBA || surf == null)
		return (((int) color.r) << 24) + (((int) color.g) << 16) + (((int) color.b) << 8) + 0x000000FF;
	#endif

	return SDL_MapRGB(surf->format, color.r, color.g, color.b);
}

SDL_Color* SDL_util::getRandomColor()
{
	SDL_Color* somecolor = new SDL_Color();
	somecolor->r = Math::randomBetween(0, 255);
	somecolor->g = Math::randomBetween(0, 255);
	somecolor->b = Math::randomBetween(0, 255);
	return somecolor;
}

SDL_Surface* SDL_util::loadBitmap(const char* path, const SDL_Color* transparentColor)
{
	SDL_Surface* bmp = SDL_LoadBMP(path);
	if(bmp != null && transparentColor != null)
		SDL_SetColorKey(bmp, SDL_SRCCOLORKEY, SDL_MapRGB(bmp->format, transparentColor->r, transparentColor->g, transparentColor->b));

	return bmp;
}

#ifdef __WIN32__
	#include <windows.h>
#endif

void SDL_util::preloadCentered()
{
	//Added macro to supply missing setenv in Win32 platform
	#ifdef __WIN32__
		SetEnvironmentVariable("SDL_VIDEO_CENTERED", "1");
	#elif defined _POSIX_VERSION
		setenv("SDL_VIDEO_CENTERED", "1", 1);
	#endif
}
