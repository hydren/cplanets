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

MidpointSolver::MidpointSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void MidpointSolver::step()
{
	const double half_timestep = timestep * 0.5;

	map<Body2D*, Vector2D> k1vs, k2vs,
							k1rs, k2rs;

	map<Body2D*, Vector2D> y0, v0;
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		y0[body] = body->position;
		v0[body] = body->velocity;
	}

	derive(k1vs, k1rs, v0, y0);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		Vector2D& k2v = k2vs[body];
		Vector2D& k2r = k2rs[body];
		const Vector2D& k1v = k1vs[body];
		const Vector2D& k1r = k1rs[body];

		k2v = body->velocity + k1v * half_timestep;
		k2r = body->position + k1r * half_timestep;
	}

	derive(k1vs, k1rs, k2vs, k2rs);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += k1vs[body] * timestep;
		body->position += k1rs[body] * timestep;
	}

	timeElapsed += timestep;
}

DEFINE_CLASS_FACTORY(ImplicitMidpointSolver, "Implicit Midpoint (RK2)");

ImplicitMidpointSolver::ImplicitMidpointSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void ImplicitMidpointSolver::step()
{
	//pre-estimation loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += body->velocity * (0.5 * timestep);
	}

	computeAccelerations();

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += body->acceleration * timestep;
		body->position += body->velocity * (0.5 * timestep);
	}

	timeElapsed += timestep;
}

DEFINE_CLASS_FACTORY(HeunSolver, "Improved Euler (Heun) (RK2)");

HeunSolver::HeunSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void HeunSolver::step()
{
	map<Body2D*, Vector2D> estimatedPosition, estimatedVelocity, estimatedAcceleration;

	computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		estimatedPosition[body] = body->position + body->velocity * timestep;
		estimatedVelocity[body] = body->velocity + body->acceleration * timestep;
	}

	computeAccelerations(estimatedAcceleration, estimatedPosition);

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += (body->velocity + estimatedVelocity[body]) * (timestep * 0.5);
		body->velocity += (body->acceleration + estimatedAcceleration[body]) * (timestep * 0.5);
	}

	timeElapsed += timestep;
}

DEFINE_CLASS_FACTORY(RalstonSolver, "Improved Euler (Ralston) (RK2)");

RalstonSolver::RalstonSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void RalstonSolver::step()
{
	map<Body2D*, Vector2D> estimatedPosition, estimatedVelocity, estimatedAcceleration;

	computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		estimatedPosition[body] = body->position + body->velocity * timestep * (2.0/3.0);
		estimatedVelocity[body] = body->velocity + body->acceleration * timestep * (2.0/3.0);
	}

	computeAccelerations(estimatedAcceleration, estimatedPosition);

	//velocity & position loop
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->position += (body->velocity + estimatedVelocity[body] * 3.0) * (timestep * 0.25);
		body->velocity += (body->acceleration + estimatedAcceleration[body] * 3.0) * (timestep * 0.25);
	}

	timeElapsed += timestep;
}
