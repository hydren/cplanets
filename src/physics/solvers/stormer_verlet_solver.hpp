/*
 * stormer_verlet_solver.hpp
 *
 *  Created on: 13 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_STORMER_VERLET_SOLVER_HPP_
#define PHYSICS_SOLVERS_STORMER_VERLET_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

#include <map>

struct StormerVerletSolver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(StormerVerletSolver);

	StormerVerletSolver(Universe2D& u);
	void step();

	protected:
	std::map<Body2D*, Vector2D> previousPositions;
};



#endif /* PHYSICS_SOLVERS_STORMER_VERLET_SOLVER_HPP_ */
