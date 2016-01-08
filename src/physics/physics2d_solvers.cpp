/*
 * physics2d_solvers.cpp
 *
 *  Created on: 21/06/2015
 *      Author: Carlos Faruolo
 */

#include "physics2d_solvers.hpp"

#include <list>
using std::list;

#include <cmath>

void AbstractPhysics2DSolver::computeAllBodiesAccelerations()
{
	foreach(Body2D&, b1, list<Body2D>, universe.bodies)
	{
//		if(b1.acceleration == NULL)
//			b1.acceleration = Vector2D();
//		else
			b1.acceleration.scale(0);

		foreach(Body2D&, b2, list<Body2D>, universe.bodies)
		{
			if(b1 != b2)
			{
				double force = -universe.gravity*b1.mass*b2.mass/pow(b1.position.distance(b2.position), 2);
				b1.acceleration.add(b1.position.difference(b2.position).normalize().scale(force/b1.mass));
			}
		}
	}
}

Vector2D AbstractPhysics2DSolver::calculateAccelerationDueToNeighborhood(Vector2D position, Body2D body)
{
	Vector2D acc;
	foreach(Body2D&, b2, list<Body2D>, universe.bodies)
	{
		if(b2 != body)
		{
			double force = -universe.gravity*body.mass*b2.mass/pow(position.distance(b2.position), 2);
			acc.add(position.difference(b2.position).normalize().scale(force/body.mass));
		}
	}
	return acc;
}

void EulerSolver::step()
{
	computeAllBodiesAccelerations();

	//velocity & position loop
	foreach(Body2D&, b1, list<Body2D>, universe.bodies)
	{
		b1.position.add(b1.velocity.times(timestep));
		b1.velocity.add(b1.acceleration.times(timestep));
	}

	timeElapsed += timestep;
}

void SemiImplicitEulerSolver::step()
{
	//position loop
	foreach(Body2D&, b1, list<Body2D>, universe.bodies)
	{
		b1.position.add(b1.velocity.times(timestep));
	}

	computeAllBodiesAccelerations();

	//velocity loop
	foreach(Body2D&, b1, list<Body2D>, universe.bodies)
	{
		b1.velocity.add(b1.acceleration.times(timestep));
		b1.position.add(b1.velocity.times(timestep));
	}

	timeElapsed += timestep;
}

void EulerCromerSolver::step()
{
	computeAllBodiesAccelerations();

	//velocity & position loop
	foreach(Body2D&, b1, list<Body2D>, universe.bodies)
	{
		b1.velocity.add(b1.acceleration.times(timestep));
		b1.position.add(b1.velocity.times(timestep));
	}

	timeElapsed += timestep;
}

void LeapfrogSolver::step()
{
	computeAllBodiesAccelerations();

	foreach(Body2D&, b1, list<Body2D>, universe.bodies)
	{
		if(timeElapsed == 0)
			b1.velocity.add(b1.acceleration.times(timestep/2));
		else
			b1.velocity.add(b1.acceleration.times(timestep));

		b1.position.add(b1.velocity.times(timestep));
	}

	timeElapsed += timestep;
}
