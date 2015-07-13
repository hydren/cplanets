/*
 * physics2d.hpp
 *
 *  Created on: 20/06/2015
 *      Author: Visagio
 */

#ifndef PHYSICS_PHYSICS2D_HPP_
#define PHYSICS_PHYSICS2D_HPP_

#include "universe2d.hpp"
#include <set>
using std::set;

struct ReferenceFrame
{
	set<Body2D*> bodies;
	Vector2D getPosition() const;
	Vector2D getVelocity() const;
};

struct Physics2D
{
	Universe2D universe;
	ReferenceFrame referenceFrame;
	AbstractPhysics2DSolver* physics2DSolver;

	void step();
	void changeReferenceFrameTo(set<Body2D*>& reference);

	struct BodyCollisionListener
	{
		virtual ~BodyCollisionListener() {}
		virtual void onBodyCollision(set<Body2D*>& collidingSet, Body2D& resultingMerger) abstract;
	};
	set<BodyCollisionListener*> registeredBodyCollisionListeners;

	private:
	set< set<Body2D*> > collisions;
	void resolveCollisions();
};

#endif /* PHYSICS_PHYSICS2D_HPP_ */
