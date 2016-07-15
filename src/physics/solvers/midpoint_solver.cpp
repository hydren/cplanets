/*
 * midpoint_solver.cpp
 *
 *  Created on: 15 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "midpoint_solver.hpp"

using std::vector;

DEFINE_CLASS_FACTORY(MidpointSolver, "Implicit Midpoint (RK2)");

MidpointSolver::MidpointSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void MidpointSolver::step()
{
	//pre-estimation loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += body->velocity * (0.5 * timestep);
	}

	computeAccelerations();

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += body->acceleration * timestep;
		body->position += body->velocity * (0.5 * timestep);
	}

	timeElapsed += timestep;
}
