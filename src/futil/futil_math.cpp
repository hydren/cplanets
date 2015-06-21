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

#ifndef FUTIL_DISABLE_RECT

//Returns true if this bounding box intersects with the given Rect.
bool Rect::intersects(const Rect& box )
{
	if
    (   ((int)this->x + (int)this->w) <= (int)box.x || //S1, S4, S7
        (int)this->x  >= ((int)box.x + (int)box.w)  || //S3, S6, S9
        ((int)this->y + (int)this->h) <= (int)box.y || //S1, S2, S3
        (int)this->y  >= ((int)box.y + (int)box.h)     //S7, S8, S9
	) return false;
	else return true;
}

Rect Rect::intersection(const Rect& box)
{
	int x, y, h, w;
	if(!intersects(box)) return Rect();
	if(this->x <= box.x){
		x = box.x;
		if(this->w >= box.x + box.w)
			w = box.w;
		else
			w = this->w - (box.x - this->x);
	}
	else{
		x = this->x;
		if(box.w >= this->x + this->w)
			w = this->w;
		else
			w = box.w - (this->x - box.x);
	}
	if(this->y <= box.y){
		y = box.y;
		if(this->h >= box.h + box.y)
			h = box.h;
		else
			h = this->h - (box.y - this->y);
	}
	else{
		y = this->y;
		if(box.h >= this->h + this->y)
			h = this->h;
		else
			h = box.h - (this->y - box.y);
	}
	Rect intersectionBox(x,y,w,h);
	return intersectionBox;
}

int Rect::area()
{
	return w*h; //you don't say
}

#endif /* FUTIL_DISABLE_RECT */
