/*
 * stormer_verlet_solver.cpp
 *
 *  Created on: 13 de jul de 2016
 *      Author: carlosfaruolo
 */

#include <physics/solvers/stormer_verlet_solver.hpp>
#include <vector>
using std::vector;

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
