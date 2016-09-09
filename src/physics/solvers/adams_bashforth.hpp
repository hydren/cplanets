/*
 * adams_bashforth.hpp
 *
 *  Created on: 29 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_ADAMS_BASHFORTH_HPP_
#define PHYSICS_SOLVERS_ADAMS_BASHFORTH_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/general/language.hpp"

#include <deque>

struct State { Vector2D velocity, acceleration; };

struct AdamsBashforthSolver extends AbstractPhysics2DSolver
{
	const unsigned steps;

	AdamsBashforthSolver(Physics2D& u, unsigned numberOfSteps, const GenericFactory* factory=null);
	~AdamsBashforthSolver();
	void step();

	protected:
	unsigned preStepsCounter;
	std::map<Body2D*, std::deque<State> > history;
	AbstractPhysics2DSolver& bootstrapSolver;
};

struct AB2Solver extends AdamsBashforthSolver
{
	DECLARE_CLASS_FACTORY(AB2Solver);
	AB2Solver(Physics2D& u);
};

struct AB3Solver extends AdamsBashforthSolver
{
	DECLARE_CLASS_FACTORY(AB3Solver);
	AB3Solver(Physics2D& u);
};

struct AB4Solver extends AdamsBashforthSolver
{
	DECLARE_CLASS_FACTORY(AB4Solver);
	AB4Solver(Physics2D& u);
};

struct AB5Solver extends AdamsBashforthSolver
{
	DECLARE_CLASS_FACTORY(AB5Solver);
	AB5Solver(Physics2D& u);
};

#endif /* PHYSICS_SOLVERS_ADAMS_BASHFORTH_HPP_ */
