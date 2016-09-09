/*
 * runge_kutta.cpp
 *
 *  Created on: 22 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "runge_kutta.hpp"

#include <stdexcept>
#include "futil/string/more_operators.hpp"

using std::string;
using std::map;
using std::vector;

#define A(i, j) butcherTable.a(i, j)
#define B(index) butcherTable.b(index)
#define C(index) butcherTable.c(index)

RungeKuttaSolver::RungeKuttaSolver(Physics2D& u, ButcherTable bt, const GenericFactory* factory)
: AbstractPhysics2DSolver(factory, u, 0.01),
  butcherTable(bt), order(bt.size-1)
{}

void RungeKuttaSolver::step()
{
	// wvs and wrs are temporary variables used to compute the velocity derivative (acceleration) and position derivative (velocity)
	// kvs[i] and krs[i] are the K's values for position and velocity for each body and for each order (1, 2, 3, ..., order)
	map<Body2D*, Vector2D> wvs, wrs, kvs[order+1], krs[order+1];

	for(unsigned i = 1; i <= order; i++)
	{
		foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
		{
			Vector2D skv, skr;
			for(unsigned j = 1; j < i; j++) //sum all previous k's (weighted according to butcher table)
			{
				if(A(i, j) == 0) continue; //don't sum if coefficient is zero (optimization)

				skv += kvs[j][body] * A(i, j); //velocity
				skr += krs[j][body] * A(i, j); //position
			}

			wvs[body] = body->velocity + skv * timestep; //velocity
			wrs[body] = body->position + skr * timestep; //position
		}

		physics.derive(kvs[i], krs[i], wvs, wrs);
	}

	// compute next position/velocity
	for(unsigned i = 1; i <= order; i++)
	{
		if(B(i) == 0) continue; //don't sum if coefficient is zero (optimization)

		//for all bodies, adds a weighted sum of k's for velocity and position
		foreach(Body2D*, body, vector<Body2D*>, physics.universe.bodies)
		{
			body->velocity += kvs[i][body] * timestep * B(i); //velocity
			body->position += krs[i][body] * timestep * B(i); //position
		}
	}

	timeElapsed += timestep;
}

// ===========================================================================================================
// Butcher table

ButcherTable::ButcherTable(unsigned size, double** data)
: size(size), data(data)
{}

double ButcherTable::a(unsigned i, unsigned j) const
{
	if(i == 0 or j == 0 or i > size-1 or j > size-1)
		throw std::out_of_range(string("ButcherTable::a() indexes out of range (")+i+", "+j+")");
	return data[i-1][j];
}

double ButcherTable::b(unsigned index) const
{
	if(index == 0 or index > size-1)
		throw std::out_of_range(string("ButcherTable::b() index out of range ")+index+")");
	return data[size-1][index];
}

double ButcherTable::c(unsigned index) const
{
	if(index == 0 or index > size-1)
		throw std::out_of_range(string("ButcherTable::c() index out of range ")+index+")");
	return data[index][0];
}
