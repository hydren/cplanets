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

namespace StandardPhysics2DSolvers
{
	void doRegister();
}

struct EulerSolver extends public AbstractPhysics2DSolver
{
	static const CustomFactory<EulerSolver> CLASS_FACTORY;

	EulerSolver(Universe2D& u);
	void step();
};

struct SemiImplicitEulerSolver extends public AbstractPhysics2DSolver
{
	static const CustomFactory<SemiImplicitEulerSolver> CLASS_FACTORY;

	SemiImplicitEulerSolver(Universe2D& u);
	void step();
};

struct EulerCromerSolver extends public AbstractPhysics2DSolver
{
	static const CustomFactory<EulerCromerSolver> CLASS_FACTORY;

	EulerCromerSolver(Universe2D& u);
	void step();
};

struct LeapfrogSolver extends public AbstractPhysics2DSolver
{
	static const CustomFactory<LeapfrogSolver> CLASS_FACTORY;

	LeapfrogSolver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_PHYSICS2D_SOLVERS_HPP_ */
