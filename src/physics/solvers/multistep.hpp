/*
 * multistep.hpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_MULTISTEP_HPP_
#define PHYSICS_SOLVERS_MULTISTEP_HPP_

#include "physics/physics2dsolver.hpp"
#include "futil/futil.hpp"

#include <map>

struct StormerVerletSolver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(StormerVerletSolver);

	StormerVerletSolver(Universe2D& u);
	void step();

	protected:
	std::map<Body2D*, Vector2D> previousPositions;
};

struct BeemanSolver extends public AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(BeemanSolver);

	BeemanSolver(Universe2D& u);
	void step();

	protected:
	std::map<Body2D*, Vector2D> previousPositions;
};

struct BackwardDifferenceCorrectionSolver extends AbstractPhysics2DSolver
{
	DECLARE_CLASS_FACTORY(BackwardDifferenceCorrectionSolver);

	BackwardDifferenceCorrectionSolver(Universe2D& u);
	void step();
	void preStep();

	protected:
	struct History
	{
		History();
		History(Body2D* body, double timestep);
		Vector2D previousPosition, previousAcceleration, previousAcceleration2;
	};

	std::map<Body2D*, History> history;
	unsigned preStepCounter;
};

#endif /* PHYSICS_SOLVERS_MULTISTEP_HPP_ */
