/*
 * leapfrog_solver.cpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "leapfrog_solver.hpp"

#include <vector>
using std::vector;

DEFINE_CLASS_FACTORY(LeapfrogSolver, "Leapfrog");

LeapfrogSolver::LeapfrogSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.1)
{}

void LeapfrogSolver::step()
{
	computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		if(timeElapsed == 0)
			body->velocity += body->acceleration * (timestep*0.5);
		else
			body->velocity += body->acceleration * timestep;

		body->position += body->velocity * timestep;
	}

	timeElapsed += timestep;
}
