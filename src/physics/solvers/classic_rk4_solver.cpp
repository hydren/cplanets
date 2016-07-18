/*
 * classic_rk4_solver.cpp
 *
 *  Created on: 14 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "classic_rk4_solver.hpp"

#include <map>
using std::map;

#include <vector>
using std::vector;

#include <cmath>

DEFINE_CLASS_FACTORY(ClassicRk4Solver, "Runge-Kutta (4th order, classic)");

ClassicRk4Solver::ClassicRk4Solver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

struct ClassicRk4Solver::deriver_aux
{
	ClassicRk4Solver& self;
	Universe2D& universe;

	deriver_aux(ClassicRk4Solver* self) :  self(*self), universe(self->universe) {}

	void operator()(map<Body2D*, Vector2D>& dvdt, map<Body2D*, Vector2D>& dydt, map<Body2D*, Vector2D>& vn, map<Body2D*, Vector2D>& yn)
	{
		self.computeAccelerations(dvdt, yn);
		dydt = vn;
	}
};

void ClassicRk4Solver::step()
{
	const double one_sixth_timestep = timestep / 6.0;
	static deriver_aux deriv(this);

	map<Body2D*, Vector2D> k1vs, k2vs, k3vs, k4vs,
							k1rs, k2rs, k3rs, k4rs;

	map<Body2D*, Vector2D> wvs, wrs;

	map<Body2D*, Vector2D> y0, v0;
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		y0[body] = body->position;
		v0[body] = body->velocity;
	}

	deriv(k1vs, k1rs, v0, y0);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		Vector2D& wv = wvs[body]; // w[0]
		Vector2D& wr = wrs[body]; // w[1]
		const Vector2D& k1v = k1vs[body]; // y[0]
		const Vector2D& k1r = k1rs[body]; // u[1]

		wv = body->velocity + k1v * (timestep * 0.5);
		wr = body->position + k1r * (timestep * 0.5);
	}

	deriv(k2vs, k2rs, wvs, wrs);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		Vector2D& wv = wvs[body]; // w[0]
		Vector2D& wr = wrs[body]; // w[1]
		const Vector2D& k2v = k2vs[body]; // y[0]
		const Vector2D& k2r = k2rs[body]; // u[1]

		wv = body->velocity + k2v * (timestep * 0.5);
		wr = body->position + k2r * (timestep * 0.5);
	}

	deriv(k3vs, k3rs, wvs, wrs);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		Vector2D& wv = wvs[body]; // w[0]
		Vector2D& wr = wrs[body]; // w[1]
		const Vector2D& k3v = k3vs[body]; // y[0]
		const Vector2D& k3r = k3rs[body]; // u[1]

		wv = body->velocity + k3v * timestep;
		wr = body->position + k3r * timestep;
	}

	deriv(k4vs, k4rs, wvs, wrs);

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->velocity += (k1vs[body] + k2vs[body]*2 + k3vs[body]*2 + k4vs[body]) * one_sixth_timestep;
		body->position += (k1rs[body] + k2rs[body]*2 + k3rs[body]*2 + k4rs[body]) * one_sixth_timestep;
	}

	timeElapsed += timestep;
}
