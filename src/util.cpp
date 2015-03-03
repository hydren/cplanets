/*
 * util.cpp
 *
 *  Created on: 03/03/2015
 *      Author: Visagio
 */

#include "util.hpp"

#include <cstdlib>

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

