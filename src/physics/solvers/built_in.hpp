/*
 * built_in.hpp
 *
 *  Created on: 12 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_SOLVERS_BUILT_IN_HPP_
#define PHYSICS_SOLVERS_BUILT_IN_HPP_

#include "euler.hpp"
#include "order2.hpp"
#include "multistep.hpp"
#include "rk2.hpp"
#include "rk3.hpp"
#include "rk4.hpp"

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

		REGISTER_CLASS_FACTORY(MidpointSolver);
		REGISTER_CLASS_FACTORY(ImplicitMidpointSolver);
		REGISTER_CLASS_FACTORY(HeunSolver);
		REGISTER_CLASS_FACTORY(RalstonSolver);

		REGISTER_CLASS_FACTORY(Rk3Solver);
		REGISTER_CLASS_FACTORY(Rk3V2Solver);

		REGISTER_CLASS_FACTORY(ClassicRk4Solver);
		REGISTER_CLASS_FACTORY(Rule38Rk4Solver);
		REGISTER_CLASS_FACTORY(GillRk4Solver);
		REGISTER_CLASS_FACTORY(RalstonRk4Solver);
	}
}



#endif /* PHYSICS_SOLVERS_BUILT_IN_HPP_ */
