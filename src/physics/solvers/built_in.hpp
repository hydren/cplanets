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
#include "midpoint_solver.hpp"
#include "classic_rk4_solver.hpp"
#include "implicit_midpoint_solver.hpp"
#include "heun_solver.hpp"
#include "ralston_solver.hpp"
#include "3_8_rk4_solver.hpp"
#include "rk3_solver.hpp"
#include "rk3_v2_solver.hpp"

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
		REGISTER_CLASS_FACTORY(MidpointSolver);
		REGISTER_CLASS_FACTORY(ImplicitMidpointSolver);
		REGISTER_CLASS_FACTORY(ClassicRk4Solver);
		REGISTER_CLASS_FACTORY(HeunSolver);
		REGISTER_CLASS_FACTORY(RalstonSolver);
		REGISTER_CLASS_FACTORY(Rule38Rk4Solver);
		REGISTER_CLASS_FACTORY(Rk3Solver);
		REGISTER_CLASS_FACTORY(Rk3V2Solver);
	}
}



#endif /* PHYSICS_SOLVERS_BUILT_IN_HPP_ */
