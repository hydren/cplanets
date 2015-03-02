/*
 * vector2d.cpp
 *
 *  Created on: 01/03/2015
 *      Author: Carlos F. M. Faruolo <5carlosfelipe5@gmail.com>felipe
 */

#include <math.h>
#include "geometry/vector2d.hpp"

bool Vector2D::operator ==(const Vector2D& v) const
{
	return v.x == x && v.y == y;
}

bool Vector2D::operator !=(const Vector2D& v) const
{
	return not ((*this) == v);
}

/** Returns true if the given vector coordinates equals the ones from this */
bool Vector2D::equals(const Vector2D& vector) const
{
	return (*this) == vector;
}

/**
 * Creates and returns a copy of this vector.
 * @return a copy of this vector.
 */
Vector2D Vector2D::clone() const
{
	return Vector2D(x, y);
}

string Vector2D::toString() const
{
	return "(" + x + ", " + y + ")";
}

double* Vector2D::getCoordinates() const
{
	double coord = new double[2];
	coord[0] = x;
	coord[1] = y;
	return coord;
}

double Vector2D::operator ~() const
{
	return sqrt( x*x + y*y );
}

/** @return the length/magnitude of this vector. */
double Vector2D::magnitude() const
{
	return ~(*this);
}
