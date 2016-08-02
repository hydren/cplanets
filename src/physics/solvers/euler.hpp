/*
 * euler.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_EULER_HPP_
#define PHYSICS_SOLVERS_EULER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct EulerSolver extends AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(EulerSolver);

	EulerSolver(Universe2D& u);
	void step();
};

struct SemiImplicitEulerSolver extends AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(SemiImplicitEulerSolver);

	SemiImplicitEulerSolver(Universe2D& u);
	void step();
};

struct EulerCromerSolver extends AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(EulerCromerSolver);

	EulerCromerSolver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_EULER_HPP_ */
