/*
 * rk_high.cpp
 *
 *  Created on: 25 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "rk_high.hpp"

DEFINE_CLASS_FACTORY(NystromRk5Solver, "Runge-Kutta (5th order, Nystrom)");
NystromRk5Solver::NystromRk5Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK5_NYSTROM, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(ButcherRk6Solver, "Runge-Kutta (6th order, Butcher)");
ButcherRk6Solver::ButcherRk6Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK6_BUTCHER, &CLASS_FACTORY)
{}
