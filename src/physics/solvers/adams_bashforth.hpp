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

struct AdamsBashforthSolver extends AbstractPhysics2DSolver
{
	const unsigned steps;

	AdamsBashforthSolver(Universe2D& u, unsigned numberOfSteps, const GenericFactory* factory=null);
	~AdamsBashforthSolver();
	void step();

	protected:
	unsigned preStepsCounter;
	std::map<Body2D*, std::deque<State> > history;
	AbstractPhysics2DSolver& bootstrapSolver;
};

struct AdamsBashforth2StepSolver extends AdamsBashforthSolver
{
	DECLARE_CLASS_FACTORY(AdamsBashforth2StepSolver);
	AdamsBashforth2StepSolver(Universe2D& u);
};

struct AdamsBashforth3StepSolver extends AdamsBashforthSolver
{
	DECLARE_CLASS_FACTORY(AdamsBashforth3StepSolver);
	AdamsBashforth3StepSolver(Universe2D& u);
};

struct AdamsBashforth4StepSolver extends AdamsBashforthSolver
{
	DECLARE_CLASS_FACTORY(AdamsBashforth4StepSolver);
	AdamsBashforth4StepSolver(Universe2D& u);
};

struct AdamsBashforth5StepSolver extends AdamsBashforthSolver
{
	DECLARE_CLASS_FACTORY(AdamsBashforth5StepSolver);
	AdamsBashforth5StepSolver(Universe2D& u);
};

#endif /* PHYSICS_SOLVERS_ADAMS_BASHFORTH_HPP_ */
