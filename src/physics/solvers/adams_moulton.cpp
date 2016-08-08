/*
 * adams_moulton.cpp
 *
 *  Created on: 8 de ago de 2016
 *      Author: felipe
 */

#include "adams_moulton.hpp"

#include <utility>

using std::vector;
using std::deque;
using std::pair;
using std::make_pair;

using Collections::coalesce2;

#define MAXSTEP 4

const double adams_moulton_coefficients[MAXSTEP][MAXSTEP+1] = {
		{ 0.5,			0.5 },
		{ 5.0/12.0, 	2.0/3.0			-1.0/12.0 },
		{ 0.375, 		19.0/24.0, 		-5.0/24.0, 		1.0/24.0},
		{ 251.0/720.0, 	646.0/720.0, 	-264.0/720.0,	106.0/720.0,	-19.0/720.0},
};  //		predict			current 		n-1				n-2				n-3				...

#define b(index) adams_moulton_coefficients[steps-1][index]

AdamsBashforthMoultonSolver::AdamsBashforthMoultonSolver(Universe2D& u, unsigned numberOfSteps, const GenericFactory* factory)
: AdamsBashforthSolver(u, numberOfSteps+1, factory)
{}

void AdamsBashforthMoultonSolver::step()
{
	if(preStepsCounter < steps)
		AdamsBashforthSolver::step();

	else
	{
		std::map<Body2D*, Vector2D > aux;
		foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
			aux[body] = body->position; // save current position

		// adams-bashforth solver will put a predition on body->position/velocity
		AdamsBashforthSolver::step();
		computeAccelerations();

		foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
		{
			// 'body->position' and 'body->velocity' are currently holding a prediction
			const State prediciton = { body->velocity, body->acceleration };

			body->position = aux[body]; // actual current position was saved beforehand
			body->velocity = history[body][0].velocity; // actual current velocity is stored at 'history[body][0]' (or 'history[body].front()')
			body->acceleration = history[body][0].acceleration; // same as above

			body->position += prediciton.velocity * b(0) + body->velocity * b(1) * timestep;
			body->velocity += prediciton.acceleration * b(0) + body->acceleration * b(1) * timestep;

			for(unsigned i = 2; i <= steps; i++)
			{
				body->position += ( history[body][i-1].velocity * b(i)) * timestep;
				body->velocity += ( history[body][i-1].acceleration * b(i)) * timestep;
			}
		}
	}
}
