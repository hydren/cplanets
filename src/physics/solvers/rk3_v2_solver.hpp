/*
 * rk3_v2_solver.hpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK3_V2_SOLVER_HPP_
#define PHYSICS_SOLVERS_RK3_V2_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct Rk3V2Solver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(Rk3V2Solver);

	Rk3V2Solver(Universe2D& u);
	void step();
};



#endif /* PHYSICS_SOLVERS_RK3_V2_SOLVER_HPP_ */
