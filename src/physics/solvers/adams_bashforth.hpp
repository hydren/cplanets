/*
 * adams_bashforth.hpp
 *
 *  Created on: 29 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_ADAMS_BASHFORTH_HPP_
#define PHYSICS_SOLVERS_ADAMS_BASHFORTH_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

#include <deque>

struct State { Vector2D velocity, acceleration; };

struct AdamsBashforthSolver extends public AbstractPhysics2DSolver
{
	const unsigned steps;

	AdamsBashforthSolver(Universe2D& u, unsigned numberOfSteps, const GenericFactory* factory=null);
	void step();

	protected:
	std::map<Body2D*, std::deque<State> > history;
};

#endif /* PHYSICS_SOLVERS_ADAMS_BASHFORTH_HPP_ */
