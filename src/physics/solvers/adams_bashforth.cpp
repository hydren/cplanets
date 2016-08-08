/*
 * adams_bashforth.cpp
 *
 *  Created on: 29 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "adams_bashforth.hpp"

#include "euler.hpp"
#include "runge_kutta.hpp"

using std::vector;
using std::deque;

using Collections::coalesce2;

#define MAXSTEP 5

const double adams_bashforth_coefficients[MAXSTEP][MAXSTEP] = {
		{ 1 },
		{ 1.5, 			-0.5 },
		{ 23.0/12.0, 	-4.0/3.0, 		5.0/12.0 },
		{ 55.0/24.0, 	-59.0/24.0, 	37.0/24.0,		-3.0/8.0},
		{ 1901.0/720.0,	-1387.0/360.0,	109.0/30.0,		-637.0/360.0,	251.0/720.0 }
};  //		current 		n-1				n-2				n-3				n-4				...

#define b(index) adams_bashforth_coefficients[steps-1][index-1]

AbstractPhysics2DSolver& chooseBootstrapSolver(unsigned numberOfSteps, Universe2D& u)
{
	AbstractPhysics2DSolver* solver;
	switch (numberOfSteps) {
		case 1: solver = new EulerCromerSolver(u); break;
		case 2: solver = new RungeKuttaSolver(u, ButcherTable::RK2_HEUN); break;
		case 3: solver = new RungeKuttaSolver(u, ButcherTable::RK3_KUTTA); break;
		case 4: solver = new RungeKuttaSolver(u, ButcherTable::RK4_CLASSIC); break;
		case 5: solver = new RungeKuttaSolver(u, ButcherTable::RK5_NYSTROM); break;
		case 6: solver = new RungeKuttaSolver(u, ButcherTable::RK6_BUTCHER); break;
		default: solver = new RungeKuttaSolver(u, ButcherTable::RK8_VERNER); break;
	}
	return *solver;
}

AdamsBashforthSolver::AdamsBashforthSolver(Universe2D& u, unsigned s, const GenericFactory* factory)
: AbstractPhysics2DSolver(factory, u, 0.01),
  steps(s > MAXSTEP? MAXSTEP : s == 0 ? 1 : s), preStepsCounter(0), history(),
  bootstrapSolver(chooseBootstrapSolver(s, u))
{}

AdamsBashforthSolver::~AdamsBashforthSolver()
{
	delete &bootstrapSolver;
}

void AdamsBashforthSolver::step()
{
	if(preStepsCounter < steps)
	{
		// store pre-steps
		foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
		{
			State state = { body->velocity, body->acceleration };
			if(history[body].size() == 0 and preStepsCounter > 0) // newly added body during bootstrap
				{ preStepsCounter = 0; return; } //reset bootstrap

			if(preStepsCounter == 0) // if bootstrap start
				history[body].clear(); // clear the history

			history[body].push_front(state);
		}

		bootstrapSolver.timeElapsed = timeElapsed;
		bootstrapSolver.step();
		timeElapsed = bootstrapSolver.timeElapsed;
		preStepsCounter++;
		return;
	}

	computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		State current = { body->velocity, body->acceleration };

		body->position += body->velocity * b(1) * timestep;
		body->velocity += body->acceleration * b(1) * timestep;

		for(unsigned i = 2; i <= steps; i++)
		{
			body->position += ( history[body][i-2].velocity * b(i)) * timestep;
			body->velocity += ( history[body][i-2].acceleration * b(i)) * timestep;
		}

		history[body].push_front(current);
		history[body].pop_back();
	}
}

DEFINE_CLASS_FACTORY(AdamsBashforth2StepSolver, "Adams-Bashforth (2-Step)");

AdamsBashforth2StepSolver::AdamsBashforth2StepSolver(Universe2D& u)
: AdamsBashforthSolver(u, 2, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(AdamsBashforth3StepSolver, "Adams-Bashforth (3-Step)");

AdamsBashforth3StepSolver::AdamsBashforth3StepSolver(Universe2D& u)
: AdamsBashforthSolver(u, 3, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(AdamsBashforth4StepSolver, "Adams-Bashforth (4-Step)");

AdamsBashforth4StepSolver::AdamsBashforth4StepSolver(Universe2D& u)
: AdamsBashforthSolver(u, 4, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(AdamsBashforth5StepSolver, "Adams-Bashforth (5-Step)");

AdamsBashforth5StepSolver::AdamsBashforth5StepSolver(Universe2D& u)
: AdamsBashforthSolver(u, 5, &CLASS_FACTORY)
{}
