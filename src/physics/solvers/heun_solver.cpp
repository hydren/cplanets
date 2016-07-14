/*
 * heun_solver.cpp
 *
 *  Created on: 13 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "heun_solver.hpp"

#include <vector>
using std::vector;

DEFINE_CLASS_FACTORY(HeunSolver, "Heun (RK2)");

HeunSolver::HeunSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void HeunSolver::step()
{
	computeAccelerations();

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += body->velocity * timestep;
		body->velocity += body->acceleration * timestep;
	}

	timeElapsed += timestep;
}
