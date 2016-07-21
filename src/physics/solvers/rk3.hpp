/*
 * rk3.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK3_HPP_
#define PHYSICS_SOLVERS_RK3_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct Rk3Solver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(Rk3Solver);

	Rk3Solver(Universe2D& u);
	void step();
};

struct Rk3V2Solver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(Rk3V2Solver);

	Rk3V2Solver(Universe2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_RK3_HPP_ */
