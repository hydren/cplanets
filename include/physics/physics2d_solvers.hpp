/*
 * physics2d_solvers.hpp
 *
 *  Created on: 21/06/2015
 *      Author: Visagio
 */

#ifndef PHYSICS_PHYSICS2D_SOLVERS_HPP_
#define PHYSICS_PHYSICS2D_SOLVERS_HPP_

#include <string>
using std::string;

#include "universe2d.hpp"

struct AbstractPhysics2DSolver
{
	string displayName;

	double timestep;

	double timeElapsed;

	Universe2D* universe;

	AbstractPhysics2DSolver(Universe2D* u)
	: universe(u), timeElapsed(0), timestep(0)
	{}

	virtual ~AbstractPhysics2DSolver();

	virtual void step() abstract;

	protected:
	void computeAllBodiesAccelerations();
	Vector2D calculateAccelerationDueToNeighborhood(Vector2D position, Body2D body);
};

#endif /* PHYSICS_PHYSICS2D_SOLVERS_HPP_ */
