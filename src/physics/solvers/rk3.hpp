/*
 * rk3.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK3_HPP_
#define PHYSICS_SOLVERS_RK3_HPP_

#include "runge_kutta.hpp"

struct KuttaRK3Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(KuttaRK3Solver);
	KuttaRK3Solver(Universe2D& u);
};

struct RK3V2Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(RK3V2Solver);
	RK3V2Solver(Universe2D& u);
};

#endif /* PHYSICS_SOLVERS_RK3_HPP_ */
