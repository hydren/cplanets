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
	EulerSolver(Universe2D& u)
	: AbstractPhysics2DSolver(u, 0.01)
	{}

	void step();
	std::string getSolverClassName() { return "EulerSolver"; }
	std::string getSolverDisplayName() { return "Euler"; }
};

struct SemiImplicitEulerSolver extends public AbstractPhysics2DSolver
{
	SemiImplicitEulerSolver(Universe2D& u)
	: AbstractPhysics2DSolver(u, 0.01)
	{}

	void step();
	std::string getSolverClassName() { return "SemiImplicitEulerSolver"; }
	std::string getSolverDisplayName() { return "Semi-implicit Euler"; }
};

struct EulerCromerSolver extends public AbstractPhysics2DSolver
{
	EulerCromerSolver(Universe2D& u)
	: AbstractPhysics2DSolver(u, 0.1)
	{}

	void step();
	std::string getSolverClassName() { return "EulerCromerSolver"; }
	std::string getSolverDisplayName() { return "Semi-implicit Euler (Euler-Cromer)"; }
};

struct LeapfrogSolver extends public AbstractPhysics2DSolver
{
	LeapfrogSolver(Universe2D& u)
	: AbstractPhysics2DSolver(u, 0.1)
	{}

	void step();
	std::string getSolverClassName() { return "LeapfrogSolver"; }
	std::string getSolverDisplayName() { return "Leapfrog"; }
};

#endif /* PHYSICS_PHYSICS2D_SOLVERS_HPP_ */
