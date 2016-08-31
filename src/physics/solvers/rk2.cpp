/*
 * rk2.cpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "rk2.hpp"

using std::vector;
using std::map;

DEFINE_CLASS_FACTORY(MidpointSolver, "Midpoint (RK2)");

MidpointSolver::MidpointSolver(Physics2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void MidpointSolver::step()
{
	map<Body2D*, Vector2D> k1vs, k2vs,
							k1rs, k2rs;

	map<Body2D*, Vector2D> wr, wv;

	physics.derive(k1vs, k1rs);

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		wv[body] = body->velocity + k1vs[body] * timestep * 0.5;
		wr[body] = body->position + k1rs[body] * timestep * 0.5;
	}

	physics.derive(k2vs, k2rs, wv, wr);

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		body->velocity += k2vs[body] * timestep;
		body->position += k2rs[body] * timestep;
	}

	timeElapsed += timestep;
}

DEFINE_CLASS_FACTORY(HeunSolver, "Improved Euler (Heun) (RK2)");

HeunSolver::HeunSolver(Physics2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void HeunSolver::step()
{
	map<Body2D*, Vector2D> estimatedPosition, estimatedVelocity, estimatedAcceleration;

	physics.computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		estimatedPosition[body] = body->position + body->velocity * timestep;
		estimatedVelocity[body] = body->velocity + body->acceleration * timestep;
	}

	physics.computeAccelerations(estimatedAcceleration, estimatedPosition);

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		body->position += (body->velocity + estimatedVelocity[body]) * (timestep * 0.5);
		body->velocity += (body->acceleration + estimatedAcceleration[body]) * (timestep * 0.5);
	}

	timeElapsed += timestep;
}

DEFINE_CLASS_FACTORY(RalstonSolver, "Improved Euler (Ralston) (RK2)");

RalstonSolver::RalstonSolver(Physics2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void RalstonSolver::step()
{
	map<Body2D*, Vector2D> estimatedPosition, estimatedVelocity, estimatedAcceleration;

	physics.computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		estimatedPosition[body] = body->position + body->velocity * timestep * (2.0/3.0);
		estimatedVelocity[body] = body->velocity + body->acceleration * timestep * (2.0/3.0);
	}

	physics.computeAccelerations(estimatedAcceleration, estimatedPosition);

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
	{
		body->position += (body->velocity + estimatedVelocity[body] * 3.0) * (timestep * 0.25);
		body->velocity += (body->acceleration + estimatedAcceleration[body] * 3.0) * (timestep * 0.25);
	}

	timeElapsed += timestep;
}
