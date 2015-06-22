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

#include "futil/futil_math.hpp"

#include <cstdlib>
#include <sstream>

/* Parses the string for matching an integer. */
int Math::parseInt(const string& str)
{
	int i;
	std::istringstream(str.c_str()) >> i;
	return i;
}

float Math::convertToMeters(float pixels)
{
	return 0.01f * pixels;
}

float Math::convertToPixels(float meters)
{
	return 100.0f * meters;
}

int randomBetween(int min, int max)
{
	if (max == min)
		return min;

	if(max < min) //swap
	{
		int t = max;
		max = min;
		min = t;
	}

	int x = rand();

	while ((max-min) < RAND_MAX && x >= RAND_MAX - (RAND_MAX % (max-min)))
		x = rand();
	return min + x % (max-min);
}
