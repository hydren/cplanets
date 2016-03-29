/*
 * universe2d.hpp
 *
 *  Created on: 20/04/2015
 *      Author: felipe
 */

#ifndef PHYSICS_UNIVERSE2D_HPP_
#define PHYSICS_UNIVERSE2D_HPP_

#include <set>
#include "../../src/physics/body2d.hpp"

struct Universe2D
{
	double gravity;
	std::list<Body2D*> bodies;

	/** Creates an empty universe with gravity value equal to 1. */
	Universe2D();

	/** Creates a deep copy of the universe, including bodies' ID. */
	Universe2D(const Universe2D& u);
};



#endif /* PHYSICS_UNIVERSE2D_HPP_ */
