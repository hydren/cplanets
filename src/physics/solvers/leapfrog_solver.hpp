/*
 * leapfrog_solver.hpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_LEAPFROG_SOLVER_HPP_
#define PHYSICS_SOLVERS_LEAPFROG_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct LeapfrogSolver extends public AbstractPhysics2DSolver
{
	static const CustomFactory<LeapfrogSolver> CLASS_FACTORY;

	LeapfrogSolver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_LEAPFROG_SOLVER_HPP_ */
