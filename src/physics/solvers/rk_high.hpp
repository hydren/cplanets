/*
 * rk_high.hpp
 *
 *  Created on: 25 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK_HIGH_HPP_
#define PHYSICS_SOLVERS_RK_HIGH_HPP_

#include "runge_kutta.hpp"

struct NystromRk5Solver extends public RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(NystromRk5Solver);
	NystromRk5Solver(Universe2D& u);
};

struct ButcherRk6Solver extends public RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(ButcherRk6Solver);
	ButcherRk6Solver(Universe2D& u);
};

#endif /* PHYSICS_SOLVERS_RK_HIGH_HPP_ */
