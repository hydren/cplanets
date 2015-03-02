/*
 * vector2d.hpp
 *
 *  Created on: 25/06/2014
 *      Author: felipe
 */

#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

struct Vector2D
{
	long double x, y;

	Vector2D(long double x=0, long double y=0)
	: x(x), y(y) {}

	bool operator ==(const Vector2D& v) const;
	bool operator !=(const Vector2D& v) const;
	bool equals(const Vector2D& v) const;

	Vector2D clone() const;

	long double operator ~() const;
	long double magnitude() const;

	Vector2D operator !() const;
	Vector2D unit() const;
	Vector2D& normalize();

	Vector2D operator -() const;
	Vector2D opposite() const;
	Vector2D& operator --();
	Vector2D& reflect();
	Vector2D& reflectX();
	Vector2D& reflectY();

	Vector2D operator +(const Vector2D& v) const;
	Vector2D sum(const Vector2D& v) const;
	Vector2D& operator +=(const Vector2D& v);
	Vector2D& add(const Vector2D& v);

	Vector2D operator -(const Vector2D& v) const;
	Vector2D difference(const Vector2D& v) const;
	Vector2D& operator -=(const Vector2D& v);
	Vector2D& subtract(const Vector2D& v);

	Vector2D operator *(const long double& factor) const;
	Vector2D times(const long double& factor) const;
	Vector2D& operator *=(const long double& factor);
	Vector2D& scale(const long double& factor);

	Vector2D operator ||(const Vector2D& v) const;
	Vector2D projection(const Vector2D& v) const;
	Vector2D rejection(const Vector2D& v) const;
	Vector2D operator |(const Vector2D& v) const;
	Vector2D reflection(const Vector2D& v) const;

	long double operator /(const Vector2D& v) const;
	long double distance(const Vector2D& v) const;

	long double operator ^(const Vector2D& v) const;
	long double innerProduct(const Vector2D& v) const;

	Vector2D operator <(const long double& radians) const;
	Vector2D rotation(const long double& radians) const;
	Vector2D& rotate(const long double& radians);
	Vector2D& operator <<(const long double& radians);
	Vector2D perpendicular() const;

	const Vector2D* const getCanonicalBase() const;
	long double* getCoordinates() const;


	/** Represents the null/zero vector. It has coordinates (0, 0). */
	const static Vector2D NULL_VECTOR;

	/** A vector codirectional with the X axis, with length 1. */
	const static Vector2D X_VERSOR;

	/** A vector codirectional with the Y axis, with length 1. */
	const static Vector2D Y_VERSOR;
};

#endif /* VECTOR2D_HPP_ */
