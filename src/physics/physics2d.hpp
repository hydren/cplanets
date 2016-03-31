/*
 * physics2d.hpp
 *
 *  Created on: 20/06/2015
 *      Author: Carlos Faruolo
 */

#ifndef PHYSICS_PHYSICS2D_HPP_
#define PHYSICS_PHYSICS2D_HPP_

#include <vector>

#include "physics2d_solvers.hpp"
#include "universe2d.hpp"

struct ReferenceFrame
{
	std::vector<Body2D*> bodies;
	Vector2D getPosition() const;
	Vector2D getVelocity() const;
};

struct Physics2D
{
	Universe2D universe;
	ReferenceFrame referenceFrame;
	AbstractPhysics2DSolver* physics2DSolver;

	void step();
	void changeReferenceFrameTo(std::vector<Body2D*>& reference);

	//callback when a collision occurs
	void (*onPhysics2DBodyCollision)(std::vector<Body2D*>& collidingList, Body2D& resultingMerger);

	private:
	std::vector< std::vector<Body2D*> > collisions;
	void resolveCollisions();
};

#endif /* PHYSICS_PHYSICS2D_HPP_ */
