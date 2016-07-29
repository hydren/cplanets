/*
 * adams_bashforth.cpp
 *
 *  Created on: 29 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "adams_bashforth.hpp"

using std::vector;

#define MAXSTEP 5

const double coefficients[][MAXSTEP] = {
		{ 1 },
		{ 1.5, 			-0.5 },
		{ 23.0/12.0, 	-4.0/3.0, 		5.0/12.0 },
		{ 55.0/24.0, 	-59.0/24.0, 	37.0/24.0,		-3.0/8.0},
		{ 1901.0/720.0,	-1387.0/360.0,	109.0/30.0,		-637.0/360.0,		251.0/720.0 }
};

#define b(index) coefficients[steps-1][index-1]

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

		body->position += body->velocity * b(1) * timestep;
		body->velocity += body->acceleration * b(1) * timestep;

		for(unsigned i = 2; i <= steps; i++)
		{
			body->position += ( history[body][i-2].velocity * b(i) * timestep ) * timestep;
			body->velocity += ( history[body][i-2].acceleration * b(i)) * timestep;
		}

		history[body].push_front(current);
		history[body].pop_back();
	}
}
