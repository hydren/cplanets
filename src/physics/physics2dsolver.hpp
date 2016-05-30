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
	Universe2D& universe;

	double timeElapsed;
	double timestep;

	AbstractPhysics2DSolver(Universe2D& u, double timestep=0)
	: universe(u), timeElapsed(0), timestep(timestep)
	{}

	virtual ~AbstractPhysics2DSolver() {}

	virtual void step()=0;
	virtual std::string getSolverClassName()=0;
	virtual std::string getSolverDisplayName()=0;

	protected:
	void computeAllBodiesAccelerations();
	Vector2D calculateAccelerationDueToNeighborhood(Vector2D position, Body2D body);
};

#endif /* PHYSICS_PHYSICS2DSOLVER_HPP_ */
