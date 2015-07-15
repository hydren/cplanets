/*
 * universe2d.cpp
 *
 *  Created on: 20/04/2015
 *      Author: felipe
 */

#include "physics/universe2d.hpp"
using std::list;

Universe2D::Universe2D(const Universe2D& u)
: gravity(1)
{
	gravity = u.gravity;
	for(list<Body2D>::const_iterator i = u.bodies.begin(); i != u.bodies.end(); ++i)
	{
		bodies.push_back(Body2D(*i));
	}
}

