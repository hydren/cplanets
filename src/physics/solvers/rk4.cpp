/*
 * rk4.cpp
 *
 *  Created on: 21 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "rk4.hpp"

DEFINE_CLASS_FACTORY(ClassicRk4Solver, "Runge-Kutta (4th order, classic)");
ClassicRk4Solver::ClassicRk4Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK4_CLASSIC, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(Rule38Rk4Solver, "Runge-Kutta (4th order, rule 3/8)");
Rule38Rk4Solver::Rule38Rk4Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK4_RULE_3_8, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(GillRk4Solver, "Runge-Kutta (4th order, Gill)");
GillRk4Solver::GillRk4Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK4_GILL, &CLASS_FACTORY)
{}

DEFINE_CLASS_FACTORY(RalstonRk4Solver, "Runge-Kutta (4th order, Ralston)");
RalstonRk4Solver::RalstonRk4Solver(Universe2D& u)
: RungeKuttaSolver(u, ButcherTable::RK4_RALSTON, &CLASS_FACTORY)
{}
