/*
 * rk3.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK3_HPP_
#define PHYSICS_SOLVERS_RK3_HPP_

#include "runge_kutta.hpp"

struct KuttaRk3Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(KuttaRk3Solver);
	KuttaRk3Solver(Universe2D& u);
};

struct Rk3V2Solver extends RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(Rk3V2Solver);
	Rk3V2Solver(Universe2D& u);
};

#endif /* PHYSICS_SOLVERS_RK3_HPP_ */
