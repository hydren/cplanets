/*
 * rk4.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK4_HPP_
#define PHYSICS_SOLVERS_RK4_HPP_

#include "runge_kutta.hpp"

struct ClassicRk4Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(ClassicRk4Solver);
	ClassicRk4Solver(Universe2D& u);
};

struct Rule38Rk4Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(Rule38Rk4Solver);
	Rule38Rk4Solver(Universe2D& u);
};

struct GillRk4Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(GillRk4Solver);
	GillRk4Solver(Universe2D& u);
};

struct RalstonRk4Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(RalstonRk4Solver);
	RalstonRk4Solver(Universe2D& u);
};

#endif /* PHYSICS_SOLVERS_RK4_HPP_ */
