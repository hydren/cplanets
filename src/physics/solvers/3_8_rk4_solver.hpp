/*
 * 3_8_rk4_solver.hpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_3_8_RK4_SOLVER_HPP_
#define PHYSICS_SOLVERS_3_8_RK4_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct Rule38Rk4Solver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(Rule38Rk4Solver);

	Rule38Rk4Solver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_3_8_RK4_SOLVER_HPP_ */
