/*
 * velocity_verlet_solver.hpp
 *
 *  Created on: 13 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_VELOCITY_VERLET_SOLVER_HPP_
#define PHYSICS_SOLVERS_VELOCITY_VERLET_SOLVER_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct VelocityVerlet extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(VelocityVerlet);

	VelocityVerlet(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_VELOCITY_VERLET_SOLVER_HPP_ */
