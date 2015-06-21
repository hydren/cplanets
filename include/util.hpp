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


//foreach useful macro
#define foreach(TYPE, ELEMENT, COLLECTION_TYPE, COLLECTION)\
for(COLLECTION_TYPE::iterator ELEMENT##MACRO_TEMP_IT = (COLLECTION).begin(); ELEMENT##MACRO_TEMP_IT != (COLLECTION).end(); ++ELEMENT##MACRO_TEMP_IT)\
for(bool ELEMENT##MACRO_TEMP_B = true; ELEMENT##MACRO_TEMP_B;)\
for(TYPE ELEMENT = *(ELEMENT##MACRO_TEMP_IT); ELEMENT##MACRO_TEMP_B; ELEMENT##MACRO_TEMP_B = false)


#endif /* UTIL_HPP_ */
