/*
 * physics2d_solvers.cpp
 *
 *  Created on: 21/06/2015
 *      Author: Carlos Faruolo
 */

#include "physics2d_solvers.hpp"

#include <cmath>
#include <vector>

using std::vector;

const AbstractPhysics2DSolver::CustomFactory<EulerSolver>
EulerSolver::CLASS_FACTORY("EulerSolver", "Euler");

EulerSolver::EulerSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void EulerSolver::step()
{
	computeAllBodiesAccelerations();

	//velocity & position loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.position.add(b1.velocity.times(timestep));
		b1.velocity.add(b1.acceleration.times(timestep));
	}

	timeElapsed += timestep;
}


const AbstractPhysics2DSolver::CustomFactory<SemiImplicitEulerSolver>
SemiImplicitEulerSolver::CLASS_FACTORY("SemiImplicitEulerSolver", "Semi-implicit Euler");

SemiImplicitEulerSolver::SemiImplicitEulerSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void SemiImplicitEulerSolver::step()
{
	//position loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.position.add(b1.velocity.times(timestep));
	}

	computeAllBodiesAccelerations();

	//velocity loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.velocity.add(b1.acceleration.times(timestep));
		b1.position.add(b1.velocity.times(timestep));
	}

	timeElapsed += timestep;
}


const AbstractPhysics2DSolver::CustomFactory<EulerCromerSolver>
EulerCromerSolver::CLASS_FACTORY("EulerCromerSolver", "Semi-implicit Euler (Euler-Cromer)");

EulerCromerSolver::EulerCromerSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.1)
{}

void EulerCromerSolver::step()
{
	computeAllBodiesAccelerations();

	//velocity & position loop
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.velocity.add(b1.acceleration.times(timestep));
		b1.position.add(b1.velocity.times(timestep));
	}

	timeElapsed += timestep;
}


const AbstractPhysics2DSolver::CustomFactory<LeapfrogSolver>
LeapfrogSolver::CLASS_FACTORY("LeapfrogSolver", "Leapfrog");

LeapfrogSolver::LeapfrogSolver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.1)
{}

void LeapfrogSolver::step()
{
	computeAllBodiesAccelerations();

	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		if(timeElapsed == 0)
			b1.velocity.add(b1.acceleration.times(timestep/2));
		else
			b1.velocity.add(b1.acceleration.times(timestep));

		b1.position.add(b1.velocity.times(timestep));
	}

	timeElapsed += timestep;
}

void StandardPhysics2DSolvers::doRegister()
{
	AbstractPhysics2DSolver::registeredFactories.push_back(&EulerSolver::CLASS_FACTORY);
	AbstractPhysics2DSolver::registeredFactories.push_back(&SemiImplicitEulerSolver::CLASS_FACTORY);
	AbstractPhysics2DSolver::registeredFactories.push_back(&EulerCromerSolver::CLASS_FACTORY);
	AbstractPhysics2DSolver::registeredFactories.push_back(&LeapfrogSolver::CLASS_FACTORY);
}
