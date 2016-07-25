/*
 * runge_kutta.hpp
 *
 *  Created on: 22 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_RUNGE_KUTTA_HPP_
#define PHYSICS_SOLVERS_RUNGE_KUTTA_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

struct ButcherTable
{
	unsigned size;
	double** data;

	ButcherTable(unsigned size, double** data);

	double a(unsigned i, unsigned j) const;
	double b(unsigned index) const;
	double c(unsigned index) const;

	static const ButcherTable CLASSIC_RK4, RULE_3_8_RK4, GILL_RK4, RALSTON_RK4;
};

/// Generic "explicit" Runge-Kutta solver (order and butcher table are specifiable)
struct RungeKuttaSolver extends public AbstractPhysics2DSolver
{
	const ButcherTable butcherTable;
	const unsigned order;

	RungeKuttaSolver(Universe2D& u, ButcherTable bt=ButcherTable::CLASSIC_RK4, const GenericFactory* factory=null);

	double a(unsigned i, unsigned j);
	double b(unsigned index);
	double c(unsigned index);

	void step();
};

#endif /* PHYSICS_SOLVERS_RUNGE_KUTTA_HPP_ */
