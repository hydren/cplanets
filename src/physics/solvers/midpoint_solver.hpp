/*
 * midpoint_solver.hpp
 *
 *  Created on: 15 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_MIDPOINT_SOLVER_HPP_
#define PHYSICS_SOLVERS_MIDPOINT_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct MidpointSolver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(MidpointSolver);

	MidpointSolver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_MIDPOINT_SOLVER_HPP_ */
