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

#include "futil/futil_string.hpp"

/** Returns a copy of the string, with leading and trailing whitespace omitted. */
string String::trim(const string& str)
{
    string str2 = str;
    size_t found;
    found = str2.find_last_not_of(" ");
    if (found != string::npos)
          str2.erase(found+1);
    else
           str2.clear();

    return str2;
}

/** Returns true if the given string str ends with the given string ending */
bool String::endsWith (string const& str, string const& ending)
{
    if (str.length() >= ending.length()) {
        return (0 == str.compare (str.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

//just to reduce redundant text
#define CONCATENATION { std::stringstream ss; ss << a << b; return ss.str(); }

string operator + (string a, string b) CONCATENATION;
string operator + (string a, int b) CONCATENATION;
string operator + (string a, long b) CONCATENATION
string operator + (string a, short b) CONCATENATION;
string operator + (string a, unsigned b) CONCATENATION;
string operator + (string a, float b) CONCATENATION;
string operator + (string a, double b) CONCATENATION;
string operator + (string a, char b) CONCATENATION;
string operator + (string a, char* b) CONCATENATION;

//backwards versions

string operator + (int a, string b) CONCATENATION;
string operator + (long a, string b) CONCATENATION;
string operator + (short a, string b) CONCATENATION;
string operator + (unsigned a, string b) CONCATENATION;
string operator + (float a, string b) CONCATENATION;
string operator + (double a, string b) CONCATENATION;
string operator + (char a, string b) CONCATENATION;
string operator + (char* a, string b) CONCATENATION;

