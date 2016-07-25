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

	ClassicRk4Solver(Universe2D& u)
	: RungeKuttaSolver(u, ButcherTable::RK4_CLASSIC, &CLASS_FACTORY)
	{}
};

struct Rule38Rk4Solver extends public RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(Rule38Rk4Solver);

	Rule38Rk4Solver(Universe2D& u)
	: RungeKuttaSolver(u, ButcherTable::RK4_RULE_3_8, &CLASS_FACTORY)
	{}
};

struct GillRk4Solver extends public RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(GillRk4Solver);

	GillRk4Solver(Universe2D& u)
	: RungeKuttaSolver(u, ButcherTable::RK4_GILL, &CLASS_FACTORY)
	{}
};

struct RalstonRk4Solver extends public RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(RalstonRk4Solver);

	RalstonRk4Solver(Universe2D& u)
	: RungeKuttaSolver(u, ButcherTable::RK4_RALSTON, &CLASS_FACTORY)
	{}
};

#endif /* PHYSICS_SOLVERS_RK4_HPP_ */
