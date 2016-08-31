/*
 * rk4.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK4_HPP_
#define PHYSICS_SOLVERS_RK4_HPP_

#include "runge_kutta.hpp"

struct ClassicRK4Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(ClassicRK4Solver);
	ClassicRK4Solver(Physics2D& u);
};

struct Rule38RK4Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(Rule38RK4Solver);
	Rule38RK4Solver(Physics2D& u);
};

struct GillRK4Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(GillRK4Solver);
	GillRK4Solver(Physics2D& u);
};

struct RalstonRK4Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(RalstonRK4Solver);
	RalstonRK4Solver(Physics2D& u);
};

#endif /* PHYSICS_SOLVERS_RK4_HPP_ */
