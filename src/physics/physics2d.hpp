/*
 * physics2d.hpp
 *
 *  Created on: 20/06/2015
 *      Author: Carlos Faruolo
 */

#ifndef PHYSICS_PHYSICS2D_HPP_
#define PHYSICS_PHYSICS2D_HPP_

#include <list>

#include "physics2d_solvers.hpp"
#include "universe2d.hpp"

struct ReferenceFrame
{
	std::list<Body2D*> bodies;
	Vector2D getPosition() const;
	Vector2D getVelocity() const;
};

struct Physics2D
{
	Universe2D universe;
	ReferenceFrame referenceFrame;
	AbstractPhysics2DSolver* physics2DSolver;

	void lock();

	void step();
	void changeReferenceFrameTo(std::list<Body2D*>& reference);


	/** A struct to notify observers of collision between bodies.
	 * XXX REMEBER TO UNREGISTER AN UNUSED LISTENER*/
	struct BodyCollisionListener
	{
		virtual ~BodyCollisionListener() {}
		virtual void onBodyCollision(std::list<Body2D*>& collidingList, Body2D& resultingMerger) abstract;
	};
	std::list<BodyCollisionListener*> registeredBodyCollisionListeners;

	private:
	std::list< std::list<Body2D*> > collisions;
	void resolveCollisions();
};

#endif /* PHYSICS_PHYSICS2D_HPP_ */
