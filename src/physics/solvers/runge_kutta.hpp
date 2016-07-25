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

/**
 *	Represents a Butcher tableau, a mnemonic device to arrange data of Runge-Kutta coefficient matrices.
 *	To specify a particular method, one needs to provide the integer s (the number of stages),
 *	and the coefficients aij (for 1 ≤ j < i ≤ s), bi (for i = 1, 2, ..., s) and ci (for i = 2, 3, ..., s).
 *	The matrix [aij] is called the Runge–Kutta matrix, while the bi and ci are known as the weights and the nodes.
 *	i.e.:
 *
 *	0
 *	c2|	a21
 *	c3|	a31	a32
 *	. |	.	.
 *	. |	.	   .
 *	. |	.	      .
 *	cs|	as1	as2...as,s-1
 *	_________________________
 *	  |	b1	b2 ...bs-1	  bs
 *
 *	Observations: this struct only works with "explicit" RK methods. "implicit" RK methods requires a further-sized table.
 *
 * */
struct ButcherTable
{
	unsigned size;
	double** data;

	/// Creates a butcher table with the given data.
	/// Note that for a order n RK method, a (n+1 x n+1)-sized matrix is needed.
	///
	/// @param size The size of the given matrix. In other words, 'data' will be used as a matrix of [size][size] dimensions.
	/// @param data An ('size'x'size')-sized matrix, with the runge-kutta coefficients.
	ButcherTable(unsigned size, double** data);

	/// Get data from the RK matrix. These are the coefficients of Kj for the computation of Ki.
	double a(unsigned i, unsigned j) const;

	/// Get the weight of the given K index.
	double b(unsigned index) const;

	/// Get the node coefficient for the computation of the given K index.
	double c(unsigned index) const;

	static const ButcherTable
		RK1_EULER,
		RK3_1, RK3_2,
		RK2_MIDPOINT, RK2_HEUN, RK2_RALSTON,
		RK4_CLASSIC, RK4_RULE_3_8, RK4_GILL, RK4_RALSTON,
		RK5_NYSTROM, RK6_BUTCHER, RK8_VERNER;
};

/// Generic "explicit" Runge-Kutta solver (order and butcher table are specifiable)
struct RungeKuttaSolver extends public AbstractPhysics2DSolver
{
	const ButcherTable butcherTable;
	const unsigned order;

	RungeKuttaSolver(Universe2D& u, ButcherTable bt=ButcherTable::RK4_CLASSIC, const GenericFactory* factory=null);

	void step();
};

#endif /* PHYSICS_SOLVERS_RUNGE_KUTTA_HPP_ */
