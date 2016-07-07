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

struct Physics2D
{
	Universe2D universe;
	AbstractPhysics2DSolver* physics2DSolver;

	Physics2D();

	void step();

	struct ReferenceFrame
	{
		friend class Physics2D;
		Vector2D position() const;
		Vector2D velocity() const;
		bool isPointLike() const;

		void reset();
		void set(Vector2D position, Vector2D velocity=Vector2D());
		void set(const Body2D** reference, unsigned n);
		void set(const std::vector<Body2D*>& reference);

		protected:
		Vector2D customPosition, customVelocity;
		std::vector<const Body2D*> bodies;
	};

	ReferenceFrame referenceFrame;

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

	protected:
	std::vector<CollisionListener*>* collisionListeners; //null unless any listener is registered

	void resolveCollisions();
};

#endif /* PHYSICS_PHYSICS2D_HPP_ */
