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

	static const ButcherTable
		RK1_EULER,
		RK2_MIDPOINT, RK2_HEUN, RK2_RALSTON,
		RK4_CLASSIC, RK4_RULE_3_8, RK4_GILL, RK4_RALSTON;
};

/// Generic "explicit" Runge-Kutta solver (order and butcher table are specifiable)
struct RungeKuttaSolver extends public AbstractPhysics2DSolver
{
	const ButcherTable butcherTable;
	const unsigned order;

	RungeKuttaSolver(Universe2D& u, ButcherTable bt=ButcherTable::RK4_CLASSIC, const GenericFactory* factory=null);

	double a(unsigned i, unsigned j);
	double b(unsigned index);
	double c(unsigned index);

	void step();
};

#endif /* PHYSICS_SOLVERS_RUNGE_KUTTA_HPP_ */
