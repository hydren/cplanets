/*
 * implicit_midpoint_solver.hpp
 *
 *  Created on: 19 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_IMPLICIT_MIDPOINT_SOLVER_HPP_
#define PHYSICS_SOLVERS_IMPLICIT_MIDPOINT_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct ImplicitMidpointSolver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(ImplicitMidpointSolver);

	ImplicitMidpointSolver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_IMPLICIT_MIDPOINT_SOLVER_HPP_ */
