/*
 * futil - Faruolo's personal C++ utility code
 *
 * Copyright 2015 Carlos Felipe M. Faruolo <5carlosfelipe5@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#ifndef FUTIL_HPP_
#define FUTIL_HPP_

#define FUTIL_VERSION "Rev1"

#ifndef FUTIL_DISABLE_CONCEPTUAL_GOODIES

//java hipster things
#define abstract =0
#define null NULL
#define extends :

/** Automatic bridge macro, to encapsulate the implementation and speed up compilation time. It also cleans the global namespace.
 * This technique is called PImpl idiom, Compiler firewall idiom, handle classes, Cheshire Cat, etc... */
#define encapsulated struct Implementation; Implementation *implementation
#define encapsulation(PARENT_CLASS) struct PARENT_CLASS::Implementation

//foreach useful macro
#define foreach(TYPE, ELEMENT, COLLECTION_TYPE, COLLECTION)\
for(COLLECTION_TYPE::iterator ELEMENT##MACRO_TEMP_IT = (COLLECTION).begin(); ELEMENT##MACRO_TEMP_IT != (COLLECTION).end(); ++ELEMENT##MACRO_TEMP_IT)\
for(bool ELEMENT##MACRO_TEMP_B = true; ELEMENT##MACRO_TEMP_B;)\
for(TYPE ELEMENT = *(ELEMENT##MACRO_TEMP_IT); ELEMENT##MACRO_TEMP_B; ELEMENT##MACRO_TEMP_B = false)

#define const_foreach(TYPE, ELEMENT, COLLECTION_TYPE, COLLECTION)\
for(COLLECTION_TYPE::const_iterator ELEMENT##MACRO_TEMP_IT = (COLLECTION).begin(); ELEMENT##MACRO_TEMP_IT != (COLLECTION).end(); ++ELEMENT##MACRO_TEMP_IT)\
for(bool ELEMENT##MACRO_TEMP_B = true; ELEMENT##MACRO_TEMP_B;)\
for(TYPE ELEMENT = *(ELEMENT##MACRO_TEMP_IT); ELEMENT##MACRO_TEMP_B; ELEMENT##MACRO_TEMP_B = false)

#include <iostream>
typedef std::ifstream FileInputStream;
typedef std::ofstream FileOutputStream;

#endif //FUTIL_DISABLE_CONCEPTUAL_GOODIES

//adds almost java-like capabilities to String. It's still not converting char*+int and alike to String
#include "futil_string.hpp"

//math-related operations
#include "futil_math.hpp"

#include <vector>
namespace Util
{
	template <typename Type>
	std::vector< std::vector <Type> > transpose(const std::vector< std::vector<Type> >& matrix)
	{
		//if empty, return a new empty
		if(matrix.size() == 0)
			return std::vector< std::vector<int> >();

		//safety check
		for(unsigned i = 0, size=matrix[0].size(); i < matrix.size(); i++)
			if(matrix[i].size() != size)
				throw_exception(string("Matrix with differing row sizes! ") + matrix[i].size());

		std::vector< std::vector<Type> > matrix_t(matrix[0].size(), std::vector<Type>(matrix.size()));

		for(unsigned i = 0; i < matrix.size(); i++)
			for(unsigned j = 0; j < matrix[i].size(); j++)
				matrix_t[j][i] = matrix[i][j];

		return matrix_t;
	}
}

//exception goodies
void throw_exception(const char* format, ...);
void throw_exception(const string& msg);
void throw_exception(const char* msg);

#endif /* FUTIL_HPP_ */
