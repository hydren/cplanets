/*
 * rk4.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK4_HPP_
#define PHYSICS_SOLVERS_RK4_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct ClassicRk4Solver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(ClassicRk4Solver);

	ClassicRk4Solver(Universe2D& u);
	void step();
};

struct Rule38Rk4Solver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(Rule38Rk4Solver);

	Rule38Rk4Solver(Universe2D& u);
	void step();
};

struct GillRk4Solver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(GillRk4Solver);

	GillRk4Solver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_RK4_HPP_ */
