/*
 * adams_moulton.hpp
 *
 *  Created on: 3 de ago de 2016
 *      Author: felipe
 */

#ifndef PHYSICS_SOLVERS_ADAMS_MOULTON_HPP_
#define PHYSICS_SOLVERS_ADAMS_MOULTON_HPP_

#include "adams_bashforth.hpp"

struct AdamsBashforthMoultonSolver extends AdamsBashforthSolver
{
	AdamsBashforthMoultonSolver(Universe2D& u, unsigned numberOfSteps, const GenericFactory* factory=null);
	void step();
};

#endif /* PHYSICS_SOLVERS_ADAMS_MOULTON_HPP_ */
