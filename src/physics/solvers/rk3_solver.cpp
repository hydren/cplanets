/*
 * rk3_solver.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "rk3_solver.hpp"

#include <map>
using std::map;

#include <vector>
using std::vector;

#include <cmath>

DEFINE_CLASS_FACTORY(Rk3Solver, "Runge-Kutta (3th order)");

Rk3Solver::Rk3Solver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void Rk3Solver::step()
{
	const double one_sixth_timestep = timestep / 6.0;

	map<Body2D*, Vector2D> k1vs, k2vs, k3vs,
							k1rs, k2rs, k3rs;

	map<Body2D*, Vector2D> wvs, wrs;

	map<Body2D*, Vector2D> y0, v0;
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		y0[body] = body->position;
		v0[body] = body->velocity;
	}

	derive(k1vs, k1rs, v0, y0);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		Vector2D& wv = wvs[body]; // w[0]
		Vector2D& wr = wrs[body]; // w[1]
		const Vector2D& k1v = k1vs[body]; // y[0]
		const Vector2D& k1r = k1rs[body]; // u[1]

		wv = body->velocity + k1v * (timestep * 0.5);
		wr = body->position + k1r * (timestep * 0.5);
	}

	derive(k2vs, k2rs, wvs, wrs);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		Vector2D& wv = wvs[body]; // w[0]
		Vector2D& wr = wrs[body]; // w[1]
		const Vector2D& k1v = k1vs[body]; // y[0]
		const Vector2D& k1r = k1rs[body]; // u[1]
		const Vector2D& k2v = k2vs[body]; // y[0]
		const Vector2D& k2r = k2rs[body]; // u[1]

		wv = body->velocity - k1v * timestep + k2v * timestep * 2.0;
		wr = body->position - k1r * timestep + k2r * timestep * 2.0;
	}

	derive(k3vs, k3rs, wvs, wrs);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += (k1vs[body] + k2vs[body]*4 + k3vs[body]) * one_sixth_timestep;
		body->position += (k1rs[body] + k2rs[body]*4 + k3rs[body]) * one_sixth_timestep;
	}

	timeElapsed += timestep;
}

