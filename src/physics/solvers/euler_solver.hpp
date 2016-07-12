/*
 * euler_solver.hpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_EULER_SOLVER_HPP_
#define PHYSICS_SOLVERS_EULER_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct EulerSolver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(EulerSolver);

	EulerSolver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_EULER_SOLVER_HPP_ */
