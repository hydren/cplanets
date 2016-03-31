/*
 * physics2d_solvers.cpp
 *
 *  Created on: 21/06/2015
 *      Author: Carlos Faruolo
 */

#include "physics2d_solvers.hpp"

#include <cmath>

#include <vector>
using std::vector;


void EulerSolver::step()
{
	computeAllBodiesAccelerations();

	//velocity & position loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.position.add(b1.velocity.times(timestep));
		b1.velocity.add(b1.acceleration.times(timestep));
	}

	timeElapsed += timestep;
}

void SemiImplicitEulerSolver::step()
{
	//position loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.position.add(b1.velocity.times(timestep));
	}

	computeAllBodiesAccelerations();

	//velocity loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.velocity.add(b1.acceleration.times(timestep));
		b1.position.add(b1.velocity.times(timestep));
	}

	timeElapsed += timestep;
}

void EulerCromerSolver::step()
{
	computeAllBodiesAccelerations();

	//velocity & position loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.velocity.add(b1.acceleration.times(timestep));
		b1.position.add(b1.velocity.times(timestep));
	}

	timeElapsed += timestep;
}

void LeapfrogSolver::step()
{
	computeAllBodiesAccelerations();

	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		if(timeElapsed == 0)
			b1.velocity.add(b1.acceleration.times(timestep/2));
		else
			b1.velocity.add(b1.acceleration.times(timestep));

		b1.position.add(b1.velocity.times(timestep));
	}

	timeElapsed += timestep;
}
