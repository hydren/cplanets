/*
 * heun_solver.hpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_HEUN_SOLVER_HPP_
#define PHYSICS_SOLVERS_HEUN_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct HeunSolver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(HeunSolver);

	HeunSolver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_HEUN_SOLVER_HPP_ */
