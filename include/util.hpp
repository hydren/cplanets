/*
 * util.hpp
 *
 *  Created on: 01/03/2015
 *      Author: Carlos F. M. Faruolo <5carlosfelipe5@gmail.com>felipe
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

#define abstract =0;

#include "utils/util_string.hpp"
#include "utils/util_exception.hpp"

template<typename T>
T max(T a, T b)
{
	return (a > b)? a : b;
}

template<typename T>
T min(T a, T b)
{
	return (a < b)? a : b;
}

int randomBetween(int min, int max);

#endif /* UTIL_HPP_ */
