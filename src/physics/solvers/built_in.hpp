/*
 * built_in.hpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_BUILT_IN_HPP_
#define PHYSICS_SOLVERS_BUILT_IN_HPP_

#include "euler.hpp"
#include "one_step.hpp"
#include "multistep.hpp"
#include "rk2.hpp"
#include "rk3.hpp"
#include "rk4.hpp"
#include "rk_high.hpp"
#include "adams_bashforth.hpp"
#include "adams_moulton.hpp"

namespace BuiltInSolvers
{
	void init()
	{
		REGISTER_CLASS_FACTORY(EulerSolver);
		REGISTER_CLASS_FACTORY(SemiImplicitEulerSolver);
		REGISTER_CLASS_FACTORY(EulerCromerSolver);

		REGISTER_CLASS_FACTORY(LeapfrogSolver);
		REGISTER_CLASS_FACTORY(VelocityVerlet);

		REGISTER_CLASS_FACTORY(StormerVerletSolver);
		REGISTER_CLASS_FACTORY(BeemanSolver);
		REGISTER_CLASS_FACTORY(BackwardDifferenceCorrectionSolver);

		REGISTER_CLASS_FACTORY(MidpointSolver);
		REGISTER_CLASS_FACTORY(HeunSolver);
		REGISTER_CLASS_FACTORY(RalstonSolver);

		REGISTER_CLASS_FACTORY(KuttaRk3Solver);
		REGISTER_CLASS_FACTORY(Rk3V2Solver);

		REGISTER_CLASS_FACTORY(ClassicRk4Solver);
		REGISTER_CLASS_FACTORY(Rule38Rk4Solver);
		REGISTER_CLASS_FACTORY(GillRk4Solver);
		REGISTER_CLASS_FACTORY(RalstonRk4Solver);

		REGISTER_CLASS_FACTORY(NystromRk5Solver);
		REGISTER_CLASS_FACTORY(ButcherRk6Solver);
		REGISTER_CLASS_FACTORY(VernerRk8Solver);

		REGISTER_CLASS_FACTORY(AB2Solver);
		REGISTER_CLASS_FACTORY(AB3Solver);
		REGISTER_CLASS_FACTORY(AB4Solver);
		REGISTER_CLASS_FACTORY(AB5Solver);

		REGISTER_CLASS_FACTORY(ABM2Solver);
		REGISTER_CLASS_FACTORY(ABM3Solver);
		REGISTER_CLASS_FACTORY(ABM4Solver);
		REGISTER_CLASS_FACTORY(ABM5Solver);
	}
}

#endif /* PHYSICS_SOLVERS_BUILT_IN_HPP_ */
