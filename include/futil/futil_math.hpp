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

#ifndef FUTIL_MATH_HPP_
#define FUTIL_MATH_HPP_

#include "futil_string.hpp"

namespace Math
{
	const double PI = 3.14159265358979323846;

	inline int abs(int i)
	{
		return i < 0 ? -i : i;
	}

	/** Parses the string for matching an integer. */
	int parseInt(const string& str);

	float convertToMeters(float pixels);

	float convertToPixels(float meters);
}

int randomBetween(int min, int max);

#endif /* FUTIL_MATH_HPP_ */
