/*
 * euler_solver.cpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "euler_solver.hpp"

#include <vector>
using std::vector;

const AbstractPhysics2DSolver::CustomFactory<EulerSolver>
EulerSolver::CLASS_FACTORY("EulerSolver", "Euler");

EulerSolver::EulerSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void EulerSolver::step()
{
	computeAllBodiesAccelerations();

	//velocity & position loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.position += b1.velocity * timestep;
		b1.velocity += b1.acceleration * timestep;
	}

	timeElapsed += timestep;
}


