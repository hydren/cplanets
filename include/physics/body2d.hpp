/*
 * body2d.hpp
 *
 *  Created on: 20/04/2015
 *      Author: felipe
 */

#ifndef PHYSICS_BODY2D_HPP_
#define PHYSICS_BODY2D_HPP_

#include "util.hpp"
#include "geometry/vector2d.hpp"

struct Body2D
{
	//generic body information

	string id;

	int color;

	double mass;

	double diameter;

	//2d geometric information

	Vector2D position;

	Vector2D velocity;

	Vector2D acceleration;

	//extra

	void* userObject;

	//constructors

	Body2D();
	Body2D(double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration);
	Body2D(string id, int c, double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration);

	/** Copy constructor (deep). Creates a new Body2D instance with the same properties (including ID). */
	Body2D(const Body2D& b);

	string toString();

	bool operator<(const Body2D& b) const;
};



#endif /* PHYSICS_BODY2D_HPP_ */
