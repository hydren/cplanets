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

DEFINE_CLASS_FACTORY(StormerVerletSolver, "Stormer-Verlet (Explicit Central Diff.)");

StormerVerletSolver::StormerVerletSolver(Physics2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01),
  previousPositions(), previousTimestep(timestep)
{}

void StormerVerletSolver::step()
{
	physics.computeAccelerations();

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		Vector2D curr = body->position;

		// get the previous position. if absent, estimate it through formula x[n-1] = x[n] - h*v[x]
		Vector2D& prev = Collections::coalesce2(previousPositions, body, (body->position-(body->velocity*timestep)));

		if(timestep != previousTimestep and timeElapsed > 0) // adjusted formula to compensate for ocasional timestep variation
			body->position += (body->position - prev) * (timestep/previousTimestep) + body->acceleration * timestep * (timestep + previousTimestep) * 0.5;
		else
			(body->position *= 2) += (body->acceleration * timestep * timestep) - prev;

		//estimated velocity
		body->velocity = (body->position - curr) * (1.0/timestep);

		prev = curr; // remember the previous position
		// previousPositions[body] = curr;

		// theory:
		// x[n+1] = 2*x[n] - x[n-1] + a[n]*h^2
		// x[n-1] = x[n]
		// v[n+1] ~~ v[n+1/2] = (x[n+1] - x[n])/h, (plus something O(h^2))
	}

	timeElapsed += timestep;
	previousTimestep = timestep;
}


DEFINE_CLASS_FACTORY(BeemanSolver, "Beeman");

BeemanSolver::BeemanSolver(Physics2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01),
  previousPositions()
{}

void BeemanSolver::step()
{
	map<Body2D*, Vector2D> accPrev, accCurr;

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
		Collections::coalesce2(previousPositions, body, (body->position-(body->velocity*timestep))); // ensure presence of a previous position

	physics.computeAccelerations(accPrev, previousPositions);
	physics.computeAccelerations(accCurr);

	//position loop
	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		Vector2D curr = body->position;

		// get the previous position. if absent, estimate it through formula x[n-1] = x[n] - h*v[x]
		Vector2D& prev = previousPositions[body];

		body->position += body->velocity*timestep + (accCurr[body]*4 - accPrev[body])*pow(timestep, 2)*(1.0/6.0);

		prev = curr; // remember the previous position
	}

	physics.computeAccelerations();

	//velocity loop
	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		body->velocity += (body->acceleration*2 + accCurr[body]*5 - accPrev[body])*timestep*(1.0/6.0);
	}

	timeElapsed += timestep;
}

DEFINE_CLASS_FACTORY(BackwardDifferenceCorrectionSolver, "Backward Difference Correction");

BackwardDifferenceCorrectionSolver::BackwardDifferenceCorrectionSolver(Physics2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01),
  history(), preStepCounter(0)
{}

/// Uses RK4 as pre-steps
void BackwardDifferenceCorrectionSolver::preStep()
{
	map<Body2D*, Vector2D> k1vs, k2vs, k3vs, k4vs,
							k1rs, k2rs, k3rs, k4rs;

	map<Body2D*, Vector2D> wvs, wrs;

	physics.derive(k1vs, k1rs);

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		wvs[body] = body->velocity + k1vs[body] * (timestep * 0.5);
		wrs[body] = body->position + k1rs[body] * (timestep * 0.5);
	}

	physics.derive(k2vs, k2rs, wvs, wrs);

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		wvs[body] = body->velocity + k2vs[body] * (timestep * 0.5);
		wrs[body] = body->position + k2rs[body] * (timestep * 0.5);
	}

	physics.derive(k3vs, k3rs, wvs, wrs);

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		wvs[body] = body->velocity + k3vs[body] * timestep;
		wrs[body] = body->position + k3rs[body] * timestep;
	}

	physics.derive(k4vs, k4rs, wvs, wrs);

	physics.computeAccelerations(); //optional

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		History& bodyHistory = Collections::coalesce2(history, body, History(body, timestep)); // ensure presence of a previous history

		bodyHistory.previousPosition = body->position;
		bodyHistory.previousAcceleration2 = bodyHistory.previousAcceleration;
		bodyHistory.previousAcceleration = body->acceleration;

		body->velocity += (k1vs[body] + k2vs[body]*2 + k3vs[body]*2 + k4vs[body]) * (timestep/6.0);
		body->position += (k1rs[body] + k2rs[body]*2 + k3rs[body]*2 + k4rs[body]) * (timestep/6.0);
	}

	timeElapsed += timestep;
	preStepCounter++;
}

BackwardDifferenceCorrectionSolver::History::History(){}

BackwardDifferenceCorrectionSolver::History::History(Body2D* body, double timestep)
:	previousPosition(body->position - body->velocity*timestep),
	previousAcceleration(), previousAcceleration2()
{}

void BackwardDifferenceCorrectionSolver::step()
{
	if(preStepCounter < 3) { preStep(); return; }

	physics.computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		History& bodyHistory = Collections::coalesce2(history, body, History(body, timestep)); // ensure presence of a previous history

		Vector2D  &fn_1 = bodyHistory.previousAcceleration,
						&fn_2 =bodyHistory.previousAcceleration2,
						previousPosition = bodyHistory.previousPosition;

		history[body].previousPosition = body->position;

		body->position = body->position * 2 - previousPosition + (body->acceleration + (body->acceleration - fn_1*2.0 + fn_2)*(1.0/12.0))*pow(timestep, 2);

		//estimate velocity
		body->velocity = (body->position - history[body].previousPosition)*(1.0/timestep);

		fn_2 = fn_1;
		fn_1 = body->acceleration;
	}
}
