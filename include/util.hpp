/*
 * util.hpp
 *
 *  Created on: 01/03/2015
 *      Author: Carlos F. M. Faruolo <5carlosfelipe5@gmail.com>felipe
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "futil/futil.hpp"
#include <set>

namespace Collections
{
	template<typename T>
	bool containsElement(const std::set<T>& set1, const T& t2)
	{
		for(std::set<T>::iterator t = set1.begin(); t != set1.end(); ++t)
			if(t2 == *t)
				return true;

		return false;
	}

	template<typename T>
	bool containsElement(const std::set<T*>& set1, const T& t2)
	{
		for(std::set<T>::iterator t = set1.begin(); t != set1.end(); ++t)
			if(t2 == **t)
				return true;

		return false;
	}
}


#endif /* UTIL_HPP_ */
