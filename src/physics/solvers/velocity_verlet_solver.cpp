/*
 * velocity_verlet_solver.cpp
 *
 *  Created on: 13 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "velocity_verlet_solver.hpp"

#include <vector>
using std::vector;

DEFINE_CLASS_FACTORY(VelocityVerlet, "Velocity Verlet");

VelocityVerlet::VelocityVerlet(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void VelocityVerlet::step()
{
	if(timeElapsed == 0) //except for the first step, this is already calculated by the second part of the step
		computeAllBodiesAccelerations();

	//velocity(pt.1) & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += body->velocity * timestep + body->acceleration * (timestep*timestep*0.5);
		body->velocity += body->acceleration * (timestep*0.5);
	}

	//we need a(n+1)
	computeAllBodiesAccelerations();

	//velocity (pt.2) loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += body->acceleration * (timestep*0.5); // using updated acceleration
	}

	timeElapsed += timestep;
}
