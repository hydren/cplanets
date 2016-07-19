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
#include <map>

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
	/** Computes and updates all bodies' accelerations based on their mutual gravitation forces, using their current positions. */
	void computeAccelerations();

	/** Computes the acceleration (resulting from mutual gravitation forces) of all bodies, at the specified positions (instead of the bodies' current position).
	 *  The resulting accelerations are stored on 'resultingAccelerations'. */
	void computeAccelerations(std::map<Body2D*, Vector2D>& resultingAccelerations, const std::map<Body2D*, Vector2D>& positions);

	void derive(std::map<Body2D*, Vector2D>& dvdt, std::map<Body2D*, Vector2D>& dydt, std::map<Body2D*, Vector2D>& vn, std::map<Body2D*, Vector2D>& yn);

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

/// Insert this in your solver class body if you intend to use the REGISTER_CLASS_FACTORY() macro (or declare the factory directly)
#define DECLARE_CLASS_FACTORY(class_name) static const CustomFactory<class_name> CLASS_FACTORY

/// Insert this in your solver class source file if you intend to use the REGISTER_CLASS_FACTORY() macro (or define the factory directly)
#define DEFINE_CLASS_FACTORY(class_name, display_name) const AbstractPhysics2DSolver::CustomFactory<class_name> class_name::CLASS_FACTORY(#class_name, display_name)

/// Use this to make the given solver class available in AbstractPhysics2DSolver::registeredFactories. 'class_name' must be the name of a subclass of AbstractPhysics2DSolver
#define REGISTER_CLASS_FACTORY(class_name) AbstractPhysics2DSolver::registeredFactories.push_back(&class_name::CLASS_FACTORY)

#endif /* PHYSICS_PHYSICS2DSOLVER_HPP_ */
