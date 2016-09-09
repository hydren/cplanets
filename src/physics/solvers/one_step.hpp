/*
 * one_step.hpp
 *
 *  Created on: 26 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_ONE_STEP_HPP_
#define PHYSICS_SOLVERS_ONE_STEP_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/general/language.hpp"

struct LeapfrogSolver extends AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(LeapfrogSolver);

	LeapfrogSolver(Physics2D& u);
	void step();
};

struct VelocityVerletSolver extends AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(VelocityVerletSolver);

	VelocityVerletSolver(Physics2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_ONE_STEP_HPP_ */
