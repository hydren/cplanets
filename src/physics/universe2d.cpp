/*
 * universe2d.cpp
 *
 *  Created on: 20/04/2015
 *      Author: felipe
 */

#include "universe2d.hpp"

#include "futil/futil.hpp"

using std::vector;

Universe2D::Universe2D()
: gravity(1)
{}

Universe2D::Universe2D(const Universe2D& u)
: gravity(u.gravity)
{
	const_foreach(Body2D*, i, vector<Body2D*>, u.bodies)
	{
		bodies.push_back(new Body2D(*i));
	}
}

