/*
 * runge_kutta.cpp
 *
 *  Created on: 22 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "runge_kutta.hpp"

#include <stdexcept>

#include <cmath>

using std::map;
using std::vector;

RungeKuttaSolver::RungeKuttaSolver(Universe2D& u, ButcherTable bt, const GenericFactory* factory)
: AbstractPhysics2DSolver(factory, u, 0.01),
  butcherTable(bt), order(bt.size-1)
{}

double RungeKuttaSolver::a(unsigned i, unsigned j)
{
	return butcherTable.a(i, j);
}

double RungeKuttaSolver::b(unsigned index)
{
	return butcherTable.b(index);
}

double RungeKuttaSolver::c(unsigned index)
{
	return butcherTable.c(index);
}

void RungeKuttaSolver::step()
{
	// wvs and wrs are temporary variables used to compute the velocity derivative (acceleration) and position derivative (velocity)
	// kvs[i] and krs[i] are the K's values for position and velocity for each body and for each order (1, 2, 3, ..., order)
	map<Body2D*, Vector2D> wvs, wrs, kvs[order+1], krs[order+1];

	for(unsigned i = 1; i <= order; i++)
	{
		foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
		{
			Vector2D skv, skr;
			for(unsigned j = 1; j < i; j++) //sum all previous k's (weighted according to butcher table)
			{
				if(a(i, j) == 0) continue; //don't sum if coefficient is zero (optimization)

				skv += kvs[j][body] * a(i, j); //velocity
				skr += krs[j][body] * a(i, j); //position
			}

			wvs[body] = body->velocity + skv * timestep; //velocity
			wrs[body] = body->position + skr * timestep; //position
		}

		derive(kvs[i], krs[i], wvs, wrs);
	}

	// compute next position/velocity
	for(unsigned i = 1; i <= order; i++)
	{
		if(b(i) == 0) continue; //don't sum if coefficient is zero (optimization)

		//for all bodies, adds a weighted sum of k's for velocity and position
		foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
		{
			body->velocity += kvs[i][body] * b(i) * timestep; //velocity
			body->position += krs[i][body] * b(i) * timestep; //position
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

template<unsigned size>
double** copytable(const double table[][size])
{
	double** data = new double*[size];
	for(unsigned i = 0; i < size; i++)
	{
		data[i] = new double[size];
		std::copy(table[i], table[i] + size, data[i]);
	}
	return data;
}
#define CREATE_BUILTIN_TABLE(name, size) const ButcherTable ButcherTable::name(size, copytable<size>(::name))
#define X 0

// sample of tables
const double RK1_EULER[2][2] = {
		{0,	X},
		{X,	1}
};
CREATE_BUILTIN_TABLE(RK1_EULER, 2);

const double RK2_MIDPOINT[3][3] = {
		{0,		X,		X},
		{0.5,	0.5,	X},
		{X,		0,		1}
};
CREATE_BUILTIN_TABLE(RK2_MIDPOINT, 3);

const double RK2_HEUN[3][3] = {
		{0,		X,		X},
		{1,		1,		X},
		{X,		-0.5,	0.5}
};
CREATE_BUILTIN_TABLE(RK2_HEUN, 3);

const double RK2_RALSTON[3][3] = {
		{0,			X,			X},
		{2.0/3.0,	2.0/3.0,	X},
		{X,			0.25,		0.75}
};
CREATE_BUILTIN_TABLE(RK2_RALSTON, 3);

const double RK4_CLASSIC[5][5] = {
		{0,		X,			X,			X,			X},
		{0.5, 	0.5,		X,			X,			X},
		{0.5,	0,			0.5,		X,			X},
		{1,		0,			0,			1,			X},
		{X,		1.0/6.0,	1.0/3.0,	1.0/3.0, 	1.0/6.0}
};
CREATE_BUILTIN_TABLE(RK4_CLASSIC, 5);

const double RK4_RULE_3_8[5][5] = {
		{0,			X,			X,			X,			X},
		{1.0/3.0, 	1.0/3.0,	X,			X,			X},
		{2.0/3.0,	-1.0/3.0,	1,			X,			X},
		{1,			1,			-1,			1,			X},
		{X,			0.125,		0.375,		0.375, 		0.125}
};
CREATE_BUILTIN_TABLE(RK4_RULE_3_8, 5);

const double RK4_GILL[5][5] = {
		{0,			X,					X,					X,					X},
		{0.5,	 	0.5,				X,					X,					X},
		{0.5,		0.5*sqrt(2)-0.5,	1-sqrt(2)*0.5,		X,					X},
		{1,			-sqrt(2)*0.5,		1+sqrt(2)*0.5,		1,					X},
		{X,			1.0/6.0,			(2-sqrt(2))/6.0,	(2+sqrt(2))/6.0, 	1}
};
CREATE_BUILTIN_TABLE(RK4_GILL, 5);

const double RK4_RALSTON[5][5] = {
		{0,							X,								X,							X,										X},
		{0.4, 						0.4,							X,							X,										X},
		{7.0/8.0-3.0*sqrt(5)/16.0,	(-2889+1428*sqrt(5))/1024.0,	(3785-1620*sqrt(5))/1024.0,	X,										X},
		{1,							(-3365+2094*sqrt(5))/6040.0,	(-975-3046*sqrt(5))/2552.0,	(467040+203968*sqrt(5))/2400845.0,		X},
		{X,							(263 + 24*sqrt(5))/1812.0,		(125-1000*sqrt(5))/3828.0,	1024*(3346 + 1623*sqrt(5))/5924787.0, 	(30 - 4*sqrt(5))/123.0}
};
CREATE_BUILTIN_TABLE(RK4_RALSTON, 5);
































