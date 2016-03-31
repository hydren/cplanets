/*
 * physics2dsolver.hpp
 *
 *  Created on: 31 de mar de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_PHYSICS2DSOLVER_HPP_
#define PHYSICS_PHYSICS2DSOLVER_HPP_

#include <string>

#include "universe2d.hpp"

struct AbstractPhysics2DSolver
{
	std::string displayName;

	Universe2D& universe;

	double timeElapsed;

	double timestep;

	AbstractPhysics2DSolver(Universe2D& u)
	: displayName("Generic solver"), universe(u), timeElapsed(0), timestep(0)
	{}

	virtual ~AbstractPhysics2DSolver() {}

	virtual void step()=0;

	protected:
	void computeAllBodiesAccelerations();
	Vector2D calculateAccelerationDueToNeighborhood(Vector2D position, Body2D body);
};



#endif /* PHYSICS_PHYSICS2DSOLVER_HPP_ */
