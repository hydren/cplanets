/*
 * rk3.cpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "rk3.hpp"

DEFINE_CLASS_FACTORY(KuttaRK3Solver, "Runge-Kutta (3th order, Kutta)");

KuttaRK3Solver::KuttaRK3Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK3_KUTTA, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(RK3V2Solver, "Runge-Kutta (3th order, variation)");

RK3V2Solver::RK3V2Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK3_2, &CLASS_FACTORY)
{}
