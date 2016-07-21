/*
 * euler.cpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "euler.hpp"

#include <vector>
using std::vector;

DEFINE_CLASS_FACTORY(EulerSolver, "Euler");

EulerSolver::EulerSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void EulerSolver::step()
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
