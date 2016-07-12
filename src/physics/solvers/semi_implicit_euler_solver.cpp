/*
 * semi_implicit_euler_solver.cpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "semi_implicit_euler_solver.hpp"

#include <vector>
using std::vector;

const AbstractPhysics2DSolver::CustomFactory<SemiImplicitEulerSolver>
SemiImplicitEulerSolver::CLASS_FACTORY("SemiImplicitEulerSolver", "Semi-implicit Euler");

SemiImplicitEulerSolver::SemiImplicitEulerSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void SemiImplicitEulerSolver::step()
{
	//position loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.position += b1.velocity * timestep;
	}

	computeAllBodiesAccelerations();

	//velocity loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.velocity += b1.acceleration * timestep;
		b1.position += b1.velocity * timestep;
	}

	timeElapsed += timestep;
}
