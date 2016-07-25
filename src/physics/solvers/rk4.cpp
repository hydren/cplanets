/*
 * rk4.cpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "rk4.hpp"

DEFINE_CLASS_FACTORY(ClassicRk4Solver, "Runge-Kutta (4th order, classic)");
DEFINE_CLASS_FACTORY(Rule38Rk4Solver, "Runge-Kutta (4th order, rule 3/8)");
DEFINE_CLASS_FACTORY(GillRk4Solver, "Runge-Kutta (4th order, Gill)");
DEFINE_CLASS_FACTORY(RalstonRk4Solver, "Runge-Kutta (4th order, Ralston)");
