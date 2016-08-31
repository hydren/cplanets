/*
 * adams_moulton.hpp
 *
 *  Created on: 3 de ago de 2016
 *      Author: felipe
 */

#ifndef PHYSICS_SOLVERS_ADAMS_MOULTON_HPP_
#define PHYSICS_SOLVERS_ADAMS_MOULTON_HPP_

#include "adams_bashforth.hpp"

struct AdamsBashforthMoultonSolver extends AdamsBashforthSolver
{
	AdamsBashforthMoultonSolver(Physics2D& u, unsigned numberOfSteps, const GenericFactory* factory=null);
	void step();
};

struct ABM2Solver extends AdamsBashforthMoultonSolver
{
	DECLARE_CLASS_FACTORY(ABM2Solver);
	ABM2Solver(Physics2D& u);
};

struct ABM3Solver extends AdamsBashforthMoultonSolver
{
	DECLARE_CLASS_FACTORY(ABM3Solver);
	ABM3Solver(Physics2D& u);
};

struct ABM4Solver extends AdamsBashforthMoultonSolver
{
	DECLARE_CLASS_FACTORY(ABM4Solver);
	ABM4Solver(Physics2D& u);
};

struct ABM5Solver extends AdamsBashforthMoultonSolver
{
	DECLARE_CLASS_FACTORY(ABM5Solver);
	ABM5Solver(Physics2D& u);
};

#endif /* PHYSICS_SOLVERS_ADAMS_MOULTON_HPP_ */
