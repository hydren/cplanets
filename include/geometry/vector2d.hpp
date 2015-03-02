/*
 * vector2d.hpp
 *
 *  Created on: 25/06/2014
 *      Author: Carlos F. M. Faruolo <5carlosfelipe5@gmail.com>felipe
 */

#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

#include "util.hpp"

struct Vector2D
{
	double x, y;

	/** Creates a Vector2D with the given coordinates */
	Vector2D::Vector2D(double x, double y)
	: x(x), y(y) {}

	/** Creates a Vector2D with the (0, 0) coordinates */
	Vector2D::Vector2D()
	: x(0), y(0) {}

	/** Copy constructor */
	Vector2D::Vector2D(const Vector2D& v)
	: x(v.x), y(v.y) {}

	bool operator ==(const Vector2D& v) const;
	bool operator !=(const Vector2D& v) const;
	bool equals(const Vector2D& v) const;

	Vector2D clone() const;

	string toString() const;

	double* getCoordinates() const;

	double operator ~() const;

	/**	@return the length/magnitude of this vector. */
	double magnitude() const;

	inline double length() const { return magnitude(); }


	/** Represents the null/zero vector. It has coordinates (0, 0). */
	const static Vector2D NULL_VECTOR;

	/** A vector codirectional with the X axis, with length 1. */
	const static Vector2D X_VERSOR;

	/** A vector codirectional with the Y axis, with length 1. */
	const static Vector2D Y_VERSOR;
};

#endif /* VECTOR2D_HPP_ */
