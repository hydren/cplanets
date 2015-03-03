/*
 * vector2d.cpp
 *
 *  Created on: 01/03/2015
 *      Author: Carlos F. M. Faruolo <5carlosfelipe5@gmail.com>felipe
 */

#include "geometry/vector2d.hpp"

#include <math.h>

bool Vector2D::operator ==(const Vector2D& v) const
{
	return v.x == x and v.y == y;
}

bool Vector2D::operator !=(const Vector2D& v) const
{
	return v.x != x or v.y != y;
}

/** Returns true if this vector coordinates equals the ones from the given vector */
bool Vector2D::equals(const Vector2D& vector) const
{
	return (*this) == vector;
}

/** Creates and returns a copy of this vector via copy constructor */
Vector2D Vector2D::clone() const
{
	return Vector2D(x, y);
}

/** Creates a string with this vector coordinates (x, y) */
string Vector2D::toString() const
{
	return string() + "(" + x + ", " + y + ")";
}

/** Creates and returns an array with this Vectors coordinates, in correct order.  */
double* Vector2D::getCoordinates() const
{
	double* coord = new double[2];
	coord[0] = x;
	coord[1] = y;
	return coord;
}

double Vector2D::operator ~() const
{
	return sqrt( x*x + y*y );
}

/** Returns the length/magnitude of this vector. */
double Vector2D::magnitude() const
{
	return ~(*this);
}

/** Return true if x = 0 and y = 0; */
bool Vector2D::isZero() const
{
	return x == 0 and y == 0;
}

/** Creates a vector with length 1 and same direction as this vector. In other words, a new vector that is a normalized version of this vector. Note that <b>the original vector remains unchanged</b>. */
Vector2D Vector2D::unit() const
{
	return isZero()? Vector2D() : Vector2D(x/magnitude(), y/magnitude());
}

/** Divides this vector's coordinates by its length/magnitude, normalizing it.
The returned object is the vector instance itself after normalization. */
Vector2D& Vector2D::normalize()
{
	if(not isZero())
	{
		double length = magnitude();
		if(length > 0)
		{
			x /= length;
			y /= length;
		}
	}
	return *this;
}

/** Creates and returns the opposite of this vector. In other words, returns a vector with same coordinates as this, but with changed signal. Note that <b>the original vector remains unchanged</b>. */
Vector2D Vector2D::opposite() const
{
	return Vector2D(-x, -y);
}

/** Changes the signal of this vector coordinates, effectively reflecting it.
<br> The returned object is <b>the vector instance itself</b> after reflection. */
Vector2D& Vector2D::reflect()
{
	x = -x;
	y = -y;
	return *this;
}

/** Creates and returns a vector that represents the sum of this vector and the given vector. Note that <b>the original vector remains unchanged</b>.*/
Vector2D Vector2D::sum(const Vector2D& v) const
{
	return Vector2D(x + v.x, y + v.y);
}

/** Adds to this vector the given vector. In other words, it performs an addition to this vector coordinates.
<br> The returned object is <b>the vector instance itself</b> after summation. */
Vector2D& Vector2D::add(const Vector2D& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

/** Creates a vector that represents the difference/displacement of this vector and the given vector, in this order. It's useful to remember that vector subtraction is <b>not commutative</b>: a-b != b-a.
<br> Note that <b>the original vector remains unchanged</b>. */
Vector2D Vector2D::difference(const Vector2D& v) const
{
	return  v.opposite().add(*this);
}

/** Subtracts from this vector the given vector. In other words, it performs an subtraction to this vector coordinates.
It's useful to remember that vector subtraction is <b>not commutative</b>: a-b != b-a.
<br> The returned object is the <b>the vector instance itself</b> after subtraction. */
Vector2D& Vector2D::subtract(const Vector2D& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

/** Creates a vector that represents the scalar multiplication of this vector by the given factor. Note that <b>the original vector remains unchanged</b>.*/
Vector2D Vector2D::times(double factor) const
{
	return Vector2D(x * factor, y * factor);
}

/** Multiply this vectors coordinates by the given factor. The returned object is <b>the vector instance itself</b> after multiplication.*/
Vector2D& Vector2D::scale(double factor)
{
	x *= factor;
	y *= factor;
	return *this;
}

/** Compute the distance between this vector and the given vector. In other words, returns the length/magnitude of the displacement between this and the given vector. */
double Vector2D::distance(const Vector2D& v) const
{
	return difference(v).magnitude();
}

/** Compute the inner/dot product between this and the given vector. */
double Vector2D::innerProduct(const Vector2D& v) const
{
	return x*v.x + y*v.y;
}

