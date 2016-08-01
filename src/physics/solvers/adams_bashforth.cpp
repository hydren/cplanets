/*
 * adams_bashforth.cpp
 *
 *  Created on: 29 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "adams_bashforth.hpp"

#include "runge_kutta.hpp"

using std::vector;
using std::deque;

using Collections::coalesce2;

#define MAXSTEP 5

const double coefficients[][MAXSTEP] = {
		{ 1 },
		{ 1.5, 			-0.5 },
		{ 23.0/12.0, 	-4.0/3.0, 		5.0/12.0 },
		{ 55.0/24.0, 	-59.0/24.0, 	37.0/24.0,		-3.0/8.0},
		{ 1901.0/720.0,	-1387.0/360.0,	109.0/30.0,		-637.0/360.0,		251.0/720.0 }
};

#define b(index) coefficients[steps-1][index-1]

// xxx this fake history greatly degrades accuracy
deque<State> fakeHistory(Body2D* body, unsigned steps)
{
	deque<State> fake;
	State stillState = {body->velocity, Vector2D()};
	for(unsigned i = 0; i < steps; i++)
		fake.push_back(stillState);
	return fake;
}

AdamsBashforthSolver::AdamsBashforthSolver(Universe2D& u, unsigned s, const GenericFactory* factory)
: AbstractPhysics2DSolver(factory, u, 0.01),
  steps(s > MAXSTEP? MAXSTEP : s), history()
{}

void AdamsBashforthSolver::step()
{
	computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		State current = { body->velocity, body->acceleration };
		deque<State>& bodyHistory = coalesce2(history, body, fakeHistory(body, steps)); //ensure history

		body->position += body->velocity * b(1) * timestep;
		body->velocity += body->acceleration * b(1) * timestep;

		for(unsigned i = 2; i <= steps; i++)
		{
			body->position += ( bodyHistory[i-2].velocity * b(i) * timestep ) * timestep;
			body->velocity += ( bodyHistory[i-2].acceleration * b(i)) * timestep;
		}

		bodyHistory.push_front(current);
		bodyHistory.pop_back();
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

