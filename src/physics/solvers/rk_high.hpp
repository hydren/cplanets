/*
 * rk_high.hpp
 *
 *  Created on: 25 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK_HIGH_HPP_
#define PHYSICS_SOLVERS_RK_HIGH_HPP_

#include "runge_kutta.hpp"

struct NystromRK5Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(NystromRK5Solver);
	NystromRK5Solver(Universe2D& u);
};

struct ButcherRK6Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(ButcherRK6Solver);
	ButcherRK6Solver(Universe2D& u);
};

struct VernerRK8Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(VernerRK8Solver);
	VernerRK8Solver(Universe2D& u);
};

#endif /* PHYSICS_SOLVERS_RK_HIGH_HPP_ */
