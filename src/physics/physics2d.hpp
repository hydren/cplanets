/*
 * physics2d.hpp
 *
 *  Created on: 20/06/2015
 *      Author: Carlos Faruolo
 */

#ifndef PHYSICS_PHYSICS2D_HPP_
#define PHYSICS_PHYSICS2D_HPP_

#include <vector>

#include "physics2dsolver.hpp"
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

	Physics2D();

	void step();
	void changeReferenceFrameTo(std::vector<Body2D*>& reference);

	//callback when a collision occurs. ignored if null (default).
	void (*onCollision)(std::vector<Body2D*>& collidingList, Body2D& resultingMerger);

	//a collision listener. this should be subclassed and registered with addCollisionListener()
	struct CollisionListener
	{
		virtual ~CollisionListener(){}
		virtual void onCollision(std::vector<Body2D*>& collidingList, Body2D& resultingMerger)=0;
	};

	void addCollisionListener(CollisionListener* listener);
	void removeCollisionListener(CollisionListener* listener);

	private:
	std::vector< std::vector<Body2D*> > collisions;
	std::vector<CollisionListener*>* collisionListeners; //null unless any listener is registered
	void resolveCollisions();
};

#endif /* PHYSICS_PHYSICS2D_HPP_ */
