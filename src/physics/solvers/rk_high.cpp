/*
 * rk_high.cpp
 *
 *  Created on: 25 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "rk_high.hpp"

DEFINE_CLASS_FACTORY(NystromRK5Solver, "Runge-Kutta (5th order, Nystrom)");
NystromRK5Solver::NystromRK5Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK5_NYSTROM, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(ButcherRK6Solver, "Runge-Kutta (6th order, Butcher)");
ButcherRK6Solver::ButcherRK6Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK6_BUTCHER, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(VernerRK8Solver, "Runge-Kutta (8th order, Verner)");
VernerRK8Solver::VernerRK8Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK8_VERNER, &CLASS_FACTORY)
{}
