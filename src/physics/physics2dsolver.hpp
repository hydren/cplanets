/*
 * physics2dsolver.hpp
 *
 *  Created on: 31 de mar de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_PHYSICS2DSOLVER_HPP_
#define PHYSICS_PHYSICS2DSOLVER_HPP_

#include <string>
#include <vector>

#include "universe2d.hpp"

struct AbstractPhysics2DSolver
{
	Universe2D& universe;

	double timeElapsed;
	double timestep;

	/// A reference to (what should be) this solver's corresponding static factory.
	struct GenericFactory; const GenericFactory* const factory;

	AbstractPhysics2DSolver(const GenericFactory* factory, Universe2D& u, double timestep=0)
	: universe(u), timeElapsed(0), timestep(timestep), factory(factory)
	{}

	virtual ~AbstractPhysics2DSolver() {}

	virtual void step()=0;

	protected:
	void computeAllBodiesAccelerations();
	Vector2D calculateAccelerationDueToNeighborhood(Vector2D position, Body2D body);

	public:

	/// A solver factory. It has only one method which returns a new instance of a AbstractPhysics2DSolver.
	/// This class is meant to be subclassed for each AbstractPhysics2DSolver subclass, so its possible to instantiate the subclass through this factory.
	/// Each factory should instantiate a distinct subclass.
	struct GenericFactory
	{
		/// The class name of the AbstractPhysics2DSolver subclass instantiated by this factory (should be, but not enforced).
		const std::string solverClassName;

		/// A human-readable name of the AbstractPhysics2DSolver subclass instantiated by this factory (should be, but not enforced).
		const std::string solverDisplayName;

		GenericFactory(const std::string& className, const std::string& displayName)
		: solverClassName(className), solverDisplayName(displayName) {}

		/// Creates a new instance of a AbstractPhysics2DSolver derived class (since distinct is abstract). Each factory should instantiate a specific subclass.
		virtual AbstractPhysics2DSolver* createSolver(Universe2D& u) const=0;
	};

	/// A template class to ease the process of subclassing GenericFactory. The template parameter should be the desired AbstractPhysics2DSolver subclass.
	template<class SpecificSolverType>
	struct CustomFactory : GenericFactory
	{
		/// Creates a custom solver factory for the solver type specified by the template parameter.
		/// @param className should be a string representation of the template parameter class.
		/// @param displayName should be a human-readable name.
		CustomFactory(const std::string& className, const std::string& displayName)
		: GenericFactory(className, displayName) {}

		AbstractPhysics2DSolver* createSolver(Universe2D& u) const { return new SpecificSolverType(u); }
	};

	/// A collection of AbstractPhysics2DSolver factories, ready to be used. Each factory can instantiate some subclass of AbstractPhysics2DSolver.
	static std::vector<const GenericFactory*> registeredFactories;
};

#endif /* PHYSICS_PHYSICS2DSOLVER_HPP_ */
