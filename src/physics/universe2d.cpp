/*
 * universe2d.cpp
 *
 *  Created on: 20/04/2015
 *      Author: felipe
 */

#include "universe2d.hpp"

#include "futil/general/language.hpp"

using std::vector;

Universe2D::Universe2D()
: gravity(1), gExp(2.0)
{}

Universe2D::Universe2D(const Universe2D& u)
: gravity(u.gravity), gExp(u.gExp)
{
	const_foreach(Body2D*, i, vector<Body2D*>, u.bodies)
	{
		bodies.push_back(new Body2D(*i));
	}
}

