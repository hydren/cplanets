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

#include "futil/futil_listener.hpp"

struct Physics2D
{
	Universe2D universe;
	AbstractPhysics2DSolver* physics2DSolver;

	Physics2D();

	void step();

	/** Exchanges the planetarium current physics solver with the another instance, provided by the given factory. */
	void setSolver(const AbstractPhysics2DSolver::GenericFactory* solverFactory);

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

	MakeListenableAndNotifyAs(onCollision, (std::vector<Body2D*>& collidingList, Body2D& resultingMerger), (collidingList, resultingMerger));

	protected:

	void resolveCollisions();
};

#endif /* PHYSICS_PHYSICS2D_HPP_ */
