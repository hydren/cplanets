/*
 * ralston_solver.hpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RALSTON_SOLVER_HPP_
#define PHYSICS_SOLVERS_RALSTON_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct RalstonSolver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(RalstonSolver);

	RalstonSolver(Universe2D& u);
	void step();
};


#endif /* PHYSICS_SOLVERS_RALSTON_SOLVER_HPP_ */
