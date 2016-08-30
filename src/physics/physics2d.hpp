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
	enum {MERGE_ON_COLLISION, BOUNCE_ON_COLLISION} collisionMode;

	Physics2D();

	void step();

	/** Exchanges the planetarium current physics solver with the given instance. This is the proper way to do it. */
	void setSolver(AbstractPhysics2DSolver* solver);

	/** Exchanges the planetarium universe with a copy of the given instance. This is the proper way to do it. */
	void setUniverse(const Universe2D& universe);

	/** Structure holding data about needed to compute a reference frame scheme.
	 *  A ReferenceFrame can either be set as:
	 *  -  The center of mass of a given set of bodies (thus, having a net velocity and total mass);
	 *  -  A fixed point (thus having zero velocity and mass), being coined as "point-like";
	 *  -  A moving frame, with specified start position and fixed velocity (thus having zero mass), being also coined "point-like" */
	struct ReferenceFrame
	{
		friend class Physics2D;

		/** Returns the current position of this reference frame. */
		Vector2D position() const;

		/** Returns the current velocity of this reference frame. */
		Vector2D velocity() const;

		/** If this reference frame is not point-like, returns the total mass of the set of bodies from which this reference frame tracks. */
		double mass() const;

		/** Returns true if this reference frame is **not** set as the center of mass of some given set of bodies. */
		bool isPointLike() const;

		/** Sets this reference frame with default values. In other words, sets itself as a static, point-like reference frame, pointing to origin (0, 0) */
		void reset();

		/** Sets this reference frame as a moving frame with specified start position and velocity. If velocity is not specified or has zero magnitude, the reference frame is a fixed point.
		 * In both cases, this reference frame is considered "point-like" and isPointLike() should return 'true' after using this method. */
		void set(Vector2D position, Vector2D velocity=Vector2D());

		/** Sets this reference frame as the center of mass of the given set of bodies. 'reference' should be an 'n'-sized array of Body2D's pointers. */
		void set(const Body2D** reference, unsigned n);

		/** Sets this reference frame as the center of mass of the given set of bodies */
		void set(const std::vector<Body2D*>& reference);

		/** If this reference frame is not point-like and contains the given body in its set of tracked bodies, removes the given body from the tracked bodies. Otherwise this method does nothing. */
		void dissociate(const Body2D* body);

		protected:
		Vector2D customPosition, customVelocity;
		std::vector<const Body2D*> bodies;
	};

	ReferenceFrame referenceFrame;

	bool systemEnergyComputingEnabled;
	long double totalPotentialEnergy, totalKineticEnergy;
	unsigned bodyCount;

	protected:
	void resolveCollisions();
	void computeEnergy();

	void resolveCollisionsByMerging();
	void resolveCollisionsByBouncing();

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
