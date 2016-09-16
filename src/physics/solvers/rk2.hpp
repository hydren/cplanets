/*
 * rk2.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK2_HPP_
#define PHYSICS_SOLVERS_RK2_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/general/language.hpp"

struct MidpointSolver extends AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(MidpointSolver);

	MidpointSolver(Physics2D& u);
	void step();
};

struct HeunSolver extends AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(HeunSolver);

	HeunSolver(Physics2D& u);
	void step();
};

struct RalstonSolver extends AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(RalstonSolver);

	RalstonSolver(Physics2D& u);
	void step();
};

#endif /* PHYSICS_SOLVERS_RK2_HPP_ */
