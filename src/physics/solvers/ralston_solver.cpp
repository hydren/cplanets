/*
 * ralston_solver.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "ralston_solver.hpp"

using std::vector;
using std::map;

DEFINE_CLASS_FACTORY(RalstonSolver, "Improved Euler (Ralston) (RK2)");

RalstonSolver::RalstonSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void RalstonSolver::step()
{
	map<Body2D*, Vector2D> estimatedPosition, estimatedVelocity, estimatedAcceleration;

	computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		estimatedPosition[body] = body->position + body->velocity * timestep * (2.0/3.0);
		estimatedVelocity[body] = body->velocity + body->acceleration * timestep * (2.0/3.0);
	}

	computeAccelerations(estimatedAcceleration, estimatedPosition);

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += (body->velocity + estimatedVelocity[body] * 3.0) * (timestep * 0.25);
		body->velocity += (body->acceleration + estimatedAcceleration[body] * 3.0) * (timestep * 0.25);
	}

	timeElapsed += timestep;
}
