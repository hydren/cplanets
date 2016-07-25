/*
 * rk3.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RK3_HPP_
#define PHYSICS_SOLVERS_RK3_HPP_

#include "runge_kutta.hpp"

struct Rk3Solver extends public RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(Rk3Solver);
	Rk3Solver(Universe2D& u);
};

struct Rk3V2Solver extends public RungeKuttaSolver
{
	DECLARE_CLASS_FACTORY(Rk3V2Solver);
	Rk3V2Solver(Universe2D& u);
};

#endif /* PHYSICS_SOLVERS_RK3_HPP_ */
