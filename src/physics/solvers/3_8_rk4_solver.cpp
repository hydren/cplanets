/*
 * 3_8_rk4_solver.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "3_8_rk4_solver.hpp"

#include <map>
using std::map;

#include <vector>
using std::vector;

#include <cmath>

DEFINE_CLASS_FACTORY(Rule38Rk4Solver, "Runge-Kutta (4th order, rule 3/8)");

Rule38Rk4Solver::Rule38Rk4Solver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void Rule38Rk4Solver::step()
{
	const double one_eigth_timestep = timestep / 8.0;

	map<Body2D*, Vector2D> k1vs, k2vs, k3vs, k4vs,
							k1rs, k2rs, k3rs, k4rs;

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

		wv = body->velocity + k1v * (timestep/3.0);
		wr = body->position + k1r * (timestep/3.0);
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

		wv = body->velocity - k1v * (timestep/3.0) + k2v * timestep;
		wr = body->position - k1r * (timestep/3.0) + k2r * timestep;
	}

	derive(k3vs, k3rs, wvs, wrs);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		Vector2D& wv = wvs[body]; // w[0]
		Vector2D& wr = wrs[body]; // w[1]
		const Vector2D& k1v = k1vs[body]; // y[0]
		const Vector2D& k1r = k1rs[body]; // u[1]
		const Vector2D& k2v = k2vs[body]; // y[0]
		const Vector2D& k2r = k2rs[body]; // u[1]
		const Vector2D& k3v = k3vs[body]; // y[0]
		const Vector2D& k3r = k3rs[body]; // u[1]

		wv = body->velocity + k1v * timestep - k2v * timestep + k3v * timestep;
		wr = body->position + k1r * timestep - k2r * timestep + k3r * timestep;
	}

	derive(k4vs, k4rs, wvs, wrs);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += (k1vs[body] + k2vs[body]*3 + k3vs[body]*3 + k4vs[body]) * one_eigth_timestep;
		body->position += (k1rs[body] + k2rs[body]*3 + k3rs[body]*3 + k4rs[body]) * one_eigth_timestep;
	}

	timeElapsed += timestep;
}

