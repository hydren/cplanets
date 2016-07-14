/*
 * semi_implicit_euler_solver.cpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "semi_implicit_euler_solver.hpp"

#include <vector>
using std::vector;

DEFINE_CLASS_FACTORY(SemiImplicitEulerSolver, "Semi-implicit Euler");

SemiImplicitEulerSolver::SemiImplicitEulerSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void SemiImplicitEulerSolver::step()
{
	//position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += body->velocity * timestep;
	}

	computeAccelerations();

	//velocity loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += body->acceleration * timestep;
	}

	timeElapsed += timestep;
}
