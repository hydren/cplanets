/*
 * heun_solver.cpp
 *
 *  Created on: 13 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "heun_solver.hpp"

using std::vector;
using std::map;

DEFINE_CLASS_FACTORY(HeunSolver, "Improved Euler (Heun) (RK2)");

HeunSolver::HeunSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void HeunSolver::step()
{
	map<Body2D*, Vector2D> estimatedPosition, estimatedVelocity, estimatedAcceleration;

	computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		estimatedPosition[body] = body->position + body->velocity * timestep;
		estimatedVelocity[body] = body->velocity + body->acceleration * timestep;
	}

	computeAccelerations(estimatedAcceleration, estimatedPosition);

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += (body->velocity + estimatedVelocity[body]) * (timestep * 0.5);
		body->velocity += (body->acceleration + estimatedAcceleration[body]) * (timestep * 0.5);
	}

	timeElapsed += timestep;
}
