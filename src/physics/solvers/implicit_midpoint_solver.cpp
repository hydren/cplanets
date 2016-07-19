/*
 * implicit_midpoint_solver.cpp
 *
 *  Created on: 19 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "implicit_midpoint_solver.hpp"

#include <vector>
using std::vector;

DEFINE_CLASS_FACTORY(ImplicitMidpointSolver, "Implicit Midpoint (RK2)");

ImplicitMidpointSolver::ImplicitMidpointSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void ImplicitMidpointSolver::step()
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
