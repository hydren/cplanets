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

#include "futil/futil.hpp"

#include <cstdarg>
#include <cstdio>

//exception goodies

/** Throws an standard exception with the specified message */
void throw_exception(const string& msg)
{
	throw std::exception(msg.c_str());
}

/** Throws an standard exception with the specified message */
void throw_exception(const char* msg)
{
	throw std::exception(msg);
}

/** Throws an standard exception with printf's style message */
void throw_exception(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char buffer[1024];
	vsprintf(buffer, format, args);
	string msg = string(buffer);
	va_end(args);
	throw std::exception(msg.c_str());
}


