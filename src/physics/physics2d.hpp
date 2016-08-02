/*
 * physics2d.hpp
 *
 *  Created on: 20/06/2015
 *      Author: Carlos Faruolo
 */

#ifndef PHYSICS_PHYSICS2D_HPP_
#define PHYSICS_PHYSICS2D_HPP_

#include "universe2d.hpp"
#include "physics2dsolver.hpp"

#include <vector>

struct Physics2D
{
	Universe2D universe;
	AbstractPhysics2DSolver* solver;

	Physics2D();

	void step();

	/** Exchanges the planetarium current physics solver with the given instance. This is the proper way to do it. */
	void setSolver(AbstractPhysics2DSolver* solver);

	/** Exchanges the planetarium universe with a copy of the given instance. This is the proper way to do it. */
	void setUniverse(const Universe2D& universe);

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

	protected:
	void resolveCollisions();

	// ============== Observer pattern to deal with collision "events" ===============================
	public:
	struct Listener
	{
		virtual ~Listener(){}
		virtual void onCollision(std::vector<Body2D*>& collidingList, Body2D& resultingMerger) = 0;
	};
	void (*onCollisionCallback)(std::vector<Body2D*>& collidingList, Body2D& resultingMerger);
	std::vector<Listener*> listeners;

	protected:
	void notifyAll(std::vector<Body2D*>& collidingList, Body2D& resultingMerger);

	// =========== Utilities ================
	public:
	static bool containsBody(std::vector<Body2D*>& collection, const Body2D* body);
	static bool removeBody(std::vector<Body2D*>& collection, const Body2D* element);
};

#endif /* PHYSICS_PHYSICS2D_HPP_ */
