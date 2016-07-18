/*
 * euler_cromer_solver.cpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "euler_cromer_solver.hpp"

#include <vector>
using std::vector;

DEFINE_CLASS_FACTORY(EulerCromerSolver, "Semi-implicit Euler (Euler-Cromer)");

EulerCromerSolver::EulerCromerSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.1)
{}

void EulerCromerSolver::step()
{
	computeAccelerations();

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += body->acceleration * timestep;
		body->position += body->velocity * timestep;
	}

	timeElapsed += timestep;
}