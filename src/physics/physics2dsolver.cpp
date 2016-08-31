/*
 * physics2dsolver.cpp
 *
 *  Created on: 31 de mar de 2016
 *      Author: carlosfaruolo
 */

#include "physics2dsolver.hpp"

/// A collection of AbstractPhysics2DSolver factories, ready to be used. Each factory can instantiate some subclass of AbstractPhysics2DSolver.
std::vector<const AbstractPhysics2DSolver::GenericFactory*> AbstractPhysics2DSolver::registeredFactories;
