/*
 * multistep.cpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "multistep.hpp"

using std::vector;
using std::map;

#include <cmath>

DEFINE_CLASS_FACTORY(StormerVerletSolver, "Stormer-Verlet");

StormerVerletSolver::StormerVerletSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01),
  previousPositions()
{}

void StormerVerletSolver::step()
{
	computeAccelerations();

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		Vector2D curr = body->position;

		// get the previous position. if absent, estimate it through formula x[n-1] = x[n] - h*v[x]
		Vector2D& prev = Collections::coalesce2(previousPositions, body, (body->position-(body->velocity*timestep)));

		(body->position *= 2) += (body->acceleration * timestep * timestep) - prev;

		//estimated velocity
		body->velocity = (body->position - curr) * (1.0/timestep);

		prev = curr; // remember the previous position
		// previousPositions[body] = curr;

		// theory:
		// x[n+1] = 2*x[n] - x[n-1] + a[n+1]*h^2
		// x[n-1] = x[n]
		// v[n+1] ~~ v[n+1/2] = (x[n+1] - x[n])/h, (plus something O(h^2))
	}

	timeElapsed += timestep;
}


DEFINE_CLASS_FACTORY(BeemanSolver, "Beeman");

BeemanSolver::BeemanSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01),
  previousPositions()
{}

void BeemanSolver::step()
{
	map<Body2D*, Vector2D> accPrev, accCurr;

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
		Collections::coalesce2(previousPositions, body, (body->position-(body->velocity*timestep))); // ensure presence of a previous position

	computeAccelerations(accPrev, previousPositions);
	computeAccelerations(accCurr);

	//position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		Vector2D curr = body->position;

		// get the previous position. if absent, estimate it through formula x[n-1] = x[n] - h*v[x]
		Vector2D& prev = previousPositions[body];

		body->position += body->velocity*timestep + (accCurr[body]*4 - accPrev[body])*pow(timestep, 2)*(1.0/6.0);

		prev = curr; // remember the previous position
	}

	computeAccelerations();

	//velocity loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += (body->acceleration*2 + accCurr[body]*5 - accPrev[body])*timestep*(1.0/6.0);
	}

	timeElapsed += timestep;
}
