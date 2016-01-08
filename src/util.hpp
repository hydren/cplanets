/*
 * util.hpp
 *
 *  Created on: 01/03/2015
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>felipe
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "futil/futil.hpp"
#include <set>
#include <list>

namespace Collections
{
	template<typename T>
	bool containsElement(const std::set<T>& set1, const T& t2)
	{
		for(typename std::set<T>::const_iterator t = set1.begin(); t != set1.end(); ++t)
			if(t2 == *t)
				return true;

		return false;
	}

	template<typename T>
	bool containsElement(const std::set<T*>& set1, const T& t2)
	{
		for(typename std::set<T*>::const_iterator t = set1.begin(); t != set1.end(); ++t)
			if(t2 == **t)
				return true;

		return false;
	}

	template<typename T>
		bool containsElement(const std::list<T>& set1, const T& t2)
		{
			for(typename std::list<T>::const_iterator t = set1.begin(); t != set1.end(); ++t)
				if(t2 == *t)
					return true;

			return false;
		}

		template<typename T>
		bool containsElement(const std::list<T*>& set1, const T& t2)
		{
			for(typename std::list<T*>::const_iterator t = set1.begin(); t != set1.end(); ++t)
				if(t2 == **t)
					return true;

			return false;
		}
}


#endif /* UTIL_HPP_ */
