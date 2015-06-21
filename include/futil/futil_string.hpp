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

#ifndef FUTIL_STRING_HPP_
#define FUTIL_STRING_HPP_

#include <sstream>
#include <string>
#include <exception>

using std::string;

//adds almost java-like capabilities to String. It's still not converting char*+int and alike to String
string operator + (string a, string b);

string operator + (string a, int b);
string operator + (string a, long b);
string operator + (string a, short b);
string operator + (string a, unsigned b);
string operator + (string a, float b);
string operator + (string a, double b);
string operator + (string a, char b);
string operator + (string a, char* b);

//backwards versions

string operator + (int a, string b);
string operator + (long a, string b);
string operator + (short a, string b);
string operator + (unsigned a, string b);
string operator + (float a, string b);
string operator + (double a, string b);
string operator + (char a, string b);
string operator + (char* a, string b);

namespace String
{
    /** Returns a copy of the string, with leading and trailing whitespace omitted. */
    string trim(const string& str);

    /** Returns true if the given string str ends with the given string ending */
    bool endsWith (string const& str, string const& ending);

    template <typename T>
    T parse(const string& str)
    {
    	std::stringstream convertor(str);
    	T value;
    	convertor >> value;
    	if(convertor.fail())
    		throw std::exception(string("Failed to convert ") + str);
    	return value;

    }

    template <typename T>
	bool parseable(const string& str)
	{
		std::stringstream convertor(str);
		T value;
		convertor >> value;
		return not convertor.fail();
	}
}



#endif /* FUTIL_STRING_HPP_ */
