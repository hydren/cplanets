/*
 * rk4.cpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "rk4.hpp"

DEFINE_CLASS_FACTORY(ClassicRK4Solver, "Runge-Kutta (4th order, classic)");
ClassicRK4Solver::ClassicRK4Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK4_CLASSIC, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(Rule38RK4Solver, "Runge-Kutta (4th order, rule 3/8)");
Rule38RK4Solver::Rule38RK4Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK4_RULE_3_8, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(GillRK4Solver, "Runge-Kutta (4th order, Gill)");
GillRK4Solver::GillRK4Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK4_GILL, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(RalstonRK4Solver, "Runge-Kutta (4th order, Ralston)");
RalstonRK4Solver::RalstonRK4Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK4_RALSTON, &CLASS_FACTORY)
{}
