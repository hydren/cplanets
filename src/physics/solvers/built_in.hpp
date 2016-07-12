/*
 * built_in.hpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_BUILT_IN_HPP_
#define PHYSICS_SOLVERS_BUILT_IN_HPP_

#include "euler_solver.hpp"
#include "semi_implicit_euler_solver.hpp"
#include "euler_cromer_solver.hpp"
#include "leapfrog_solver.hpp"

namespace BuiltInSolvers
{
	void init()
	{
		AbstractPhysics2DSolver::registeredFactories.push_back(&EulerSolver::CLASS_FACTORY);
		AbstractPhysics2DSolver::registeredFactories.push_back(&SemiImplicitEulerSolver::CLASS_FACTORY);
		AbstractPhysics2DSolver::registeredFactories.push_back(&EulerCromerSolver::CLASS_FACTORY);
		AbstractPhysics2DSolver::registeredFactories.push_back(&LeapfrogSolver::CLASS_FACTORY);
	}
}



#endif /* PHYSICS_SOLVERS_BUILT_IN_HPP_ */
