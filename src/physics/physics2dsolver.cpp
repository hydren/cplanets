/*
 * physics2dsolver.cpp
 *
 *  Created on: 31 de mar de 2016
 *      Author: carlosfaruolo
 */

#include "physics2dsolver.hpp"

#include <cmath>

using std::vector;
using std::map;


/// A collection of AbstractPhysics2DSolver factories, ready to be used. Each factory can instantiate some subclass of AbstractPhysics2DSolver.
vector<const AbstractPhysics2DSolver::GenericFactory*> AbstractPhysics2DSolver::registeredFactories;

/** Computes and updates all bodies' accelerations based on their mutual gravitation forces, using their current positions. */
void AbstractPhysics2DSolver::computeAccelerations()
{
	vector<Body2D*>& b = universe.bodies;
	for(unsigned i = 0; i < universe.bodies.size(); i++)
	{
		b[i]->acceleration *= 0; // let acceleration be zero

		for(unsigned j = 0; j < universe.bodies.size(); j++)
		if(b[j] != b[i])
		{
			double forceMagnitude = (-universe.gravity * b[i]->mass * b[j]->mass) / pow(b[i]->position % b[j]->position, 2); // % is a distance operator
			b[i]->acceleration += (b[i]->position - b[j]->position).normalize() * (forceMagnitude/b[i]->mass);
		}
	}
}

/** Computes the acceleration (resulting from mutual gravitation forces) of all bodies, using their current positions.
 *  The resulting accelerations are stored on 'resultingAccelerations'. */
void AbstractPhysics2DSolver::computeAccelerations(map<Body2D*, Vector2D>& acc)
{
	vector<Body2D*>& b = universe.bodies;
	for(unsigned i = 0; i < universe.bodies.size(); i++)
	{
		acc[b[i]] *= 0; // let acceleration be zero

		for(unsigned j = 0; j < universe.bodies.size(); j++)
		if(b[i] != b[j])
		{
			double forceScalar = (-universe.gravity * b[i]->mass * b[j]->mass) / pow(b[i]->position.distance(b[j]->position), 2);
			acc[b[i]] += (b[i]->position - b[j]->position).normalize() * (forceScalar/b[i]->mass);
		}
	}
}

/** Computes the acceleration (resulting from mutual gravitation forces) of all bodies, at the specified positions (instead of the bodies' current position).
 *  The resulting accelerations are stored on 'resultingAccelerations'. */
void AbstractPhysics2DSolver::computeAccelerations(map<Body2D*, Vector2D>& acc, map<Body2D*, Vector2D>& pos)
{
	vector<Body2D*>& b = universe.bodies;
	for(unsigned i = 0; i < universe.bodies.size(); i++)
	{
		acc[b[i]] *= 0; // let acceleration be zero

		for(unsigned j = 0; j < universe.bodies.size(); j++)
		if(b[i] != b[j])
		{
			double forceScalar = (-universe.gravity * b[i]->mass * b[j]->mass) / pow(pos[b[i]].distance(pos[b[j]]), 2);
			acc[b[i]] += (pos[b[i]] - pos[b[j]]).normalize() * (forceScalar/b[i]->mass);
		}
	}
}

void AbstractPhysics2DSolver::derive(std::map<Body2D*, Vector2D>& dvdt, std::map<Body2D*, Vector2D>& dydt)
{
	computeAccelerations(dvdt);

	for(unsigned i = 0; i < universe.bodies.size(); i++)
		dydt[universe.bodies[i]] = universe.bodies[i]->velocity;
}

void AbstractPhysics2DSolver::derive(map<Body2D*, Vector2D>& dvdt, map<Body2D*, Vector2D>& dydt, map<Body2D*, Vector2D>& vn, map<Body2D*, Vector2D>& yn)
{
	computeAccelerations(dvdt, yn);
	dydt = vn;
}
