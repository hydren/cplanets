/*
 * physics2d_solvers.hpp
 *
 *  Created on: 21/06/2015
 *      Author: Carlos Faruolo
 */

#ifndef PHYSICS_PHYSICS2D_SOLVERS_HPP_
#define PHYSICS_PHYSICS2D_SOLVERS_HPP_

#include <string>

#include "physics2dsolver.hpp"
#include "futil/futil.hpp"


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
