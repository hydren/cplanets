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
#include "stormer_verlet_solver.hpp"
#include "velocity_verlet_solver.hpp"

namespace BuiltInSolvers
{
	void init()
	{
		REGISTER_CLASS_FACTORY(EulerSolver);
		REGISTER_CLASS_FACTORY(SemiImplicitEulerSolver);
		REGISTER_CLASS_FACTORY(EulerCromerSolver);
		REGISTER_CLASS_FACTORY(LeapfrogSolver);
		REGISTER_CLASS_FACTORY(StormerVerletSolver);
		REGISTER_CLASS_FACTORY(VelocityVerlet);
	}
}



#endif /* PHYSICS_SOLVERS_BUILT_IN_HPP_ */
