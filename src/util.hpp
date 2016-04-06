/*
 * util.hpp
 *
 *  Created on: 01/03/2015
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>felipe
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "futil/futil.hpp"

#include "hotfixes.h"

#define synchronized(mutex) for(bool mutex##_SYNCHRONIZED=SDL_mutexP(mutex)||true;mutex##_SYNCHRONIZED;mutex##_SYNCHRONIZED=SDL_mutexV(mutex)&&false)

//needed to display the version
extern const std::string CPLANETS_VERSION;


#endif /* UTIL_HPP_ */
