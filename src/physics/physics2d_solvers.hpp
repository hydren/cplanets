/*
 * physics2d_solvers.hpp
 *
 *  Created on: 21/06/2015
 *      Author: Carlos Faruolo
 */

#ifndef PHYSICS_PHYSICS2D_SOLVERS_HPP_
#define PHYSICS_PHYSICS2D_SOLVERS_HPP_

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

	virtual void step() abstract;

	protected:
	void computeAllBodiesAccelerations();
	Vector2D calculateAccelerationDueToNeighborhood(Vector2D position, Body2D body);
};

struct EulerSolver extends public AbstractPhysics2DSolver
{
	EulerSolver(Universe2D& u) : AbstractPhysics2DSolver(u)
	{
		this->displayName = "Euler";
		this->timestep = 0.01;
	}

	void step();
};

struct SemiImplicitEulerSolver extends public AbstractPhysics2DSolver
{
	SemiImplicitEulerSolver(Universe2D& u) : AbstractPhysics2DSolver(u)
	{
		this->displayName = "Semi-implicit Euler";
		this->timestep = 0.01;
	}

	void step();
};

struct EulerCromerSolver extends public AbstractPhysics2DSolver
{
	EulerCromerSolver(Universe2D& u) : AbstractPhysics2DSolver(u)
	{
		this->displayName = "Semi-implicit Euler (Euler-Cromer)";
		this->timestep = 0.1;
	}

	void step();
};

struct LeapfrogSolver extends public AbstractPhysics2DSolver
{
	LeapfrogSolver(Universe2D& u) : AbstractPhysics2DSolver(u)
	{
		this->displayName = "Leapfrog";
		this->timestep = 0.1;
	}

	void step();
};

#endif /* PHYSICS_PHYSICS2D_SOLVERS_HPP_ */
