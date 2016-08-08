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

DEFINE_CLASS_FACTORY(HeunRK3Solver, "Runge-Kutta (3th order, Heun)");

HeunRK3Solver::HeunRK3Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK3_HEUN, &CLASS_FACTORY)
{}
