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


#endif /* SDL_UTIL_HPP_ */
