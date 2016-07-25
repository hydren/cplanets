/*
 * rk3.cpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "rk3.hpp"

DEFINE_CLASS_FACTORY(Rk3Solver, "Runge-Kutta (3th order)");

Rk3Solver::Rk3Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK3_1, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(Rk3V2Solver, "Runge-Kutta (3th order, variation)");

Rk3V2Solver::Rk3V2Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK3_2, &CLASS_FACTORY)
{}
