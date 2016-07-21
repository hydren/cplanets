/*
 * order2.cpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "order2.hpp"

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

DEFINE_CLASS_FACTORY(VelocityVerlet, "Velocity Verlet");

VelocityVerlet::VelocityVerlet(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void VelocityVerlet::step()
{
	if(timeElapsed == 0) //except for the first step, this is already calculated by the second part of the step
		computeAccelerations();

	//velocity(pt.1) & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += body->velocity * timestep + body->acceleration * (timestep*timestep*0.5);
		body->velocity += body->acceleration * (timestep*0.5);
	}

	//we need a(n+1)
	computeAccelerations();

	//velocity (pt.2) loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += body->acceleration * (timestep*0.5); // using updated acceleration
	}

	timeElapsed += timestep;
}
