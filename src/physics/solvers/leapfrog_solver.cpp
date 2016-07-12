/*
 * leapfrog_solver.cpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "leapfrog_solver.hpp"

#include <vector>
using std::vector;

const AbstractPhysics2DSolver::CustomFactory<LeapfrogSolver>
LeapfrogSolver::CLASS_FACTORY("LeapfrogSolver", "Leapfrog");

LeapfrogSolver::LeapfrogSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.1)
{}

void LeapfrogSolver::step()
{
	computeAllBodiesAccelerations();

	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		if(timeElapsed == 0)
			b1.velocity += b1.acceleration * (timestep*0.5);
		else
			b1.velocity += b1.acceleration * timestep;

		b1.position += b1.velocity * timestep;
	}

	timeElapsed += timestep;
}
