/*
 * universe2d.hpp
 *
 *  Created on: 20/04/2015
 *      Author: felipe
 */

#ifndef PHYSICS_UNIVERSE2D_HPP_
#define PHYSICS_UNIVERSE2D_HPP_

#include "body2d.hpp"

#include <set>

struct Universe2D
{
	double gravity;
	std::list<Body2D> bodies;

	Universe2D()
	: gravity(1)
	{}

	/* Creates a deep copy of the universe, including bodies' ID. */
	Universe2D(const Universe2D& u);
};



#endif /* PHYSICS_UNIVERSE2D_HPP_ */
