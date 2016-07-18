/*
 * classic_rk4_solver.hpp
 *
 *  Created on: 14 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_CLASSIC_RK4_SOLVER_HPP_
#define PHYSICS_SOLVERS_CLASSIC_RK4_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct ClassicRk4Solver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(ClassicRk4Solver);

	ClassicRk4Solver(Universe2D& u);
	void step();

	struct deriver_aux;
	friend class deriver_aux;
};

#endif /* PHYSICS_SOLVERS_CLASSIC_RK4_SOLVER_HPP_ */