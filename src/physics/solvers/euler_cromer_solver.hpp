/*
 * euler_cromer_solver.hpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_EULER_CROMER_SOLVER_HPP_
#define PHYSICS_SOLVERS_EULER_CROMER_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct EulerCromerSolver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(EulerCromerSolver);

	EulerCromerSolver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_EULER_CROMER_SOLVER_HPP_ */
