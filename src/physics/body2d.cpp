/*
 * body2d.cpp
 *
 *  Created on: 20/04/2015
 *      Author: felipe
 */

#include "body2d.hpp"

#include "futil/general/language.hpp"
#include "futil/string/more_operators.hpp"
#include "futil/math/more_random.h"

#include <cmath>

using std::string;

string generateRandomName()
{
	return string("B")+random_between(0, 10000);
}

Body2D::Body2D(double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration)
: mass(mass), diameter(diameter), position(position), velocity(velocity), acceleration(acceleration), userObject(null)
{
	id = generateRandomName();
}

Body2D::Body2D(string id, double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration)
: id(id), mass(mass), diameter(diameter), position(position), velocity(velocity), acceleration(acceleration), userObject(null)
{}

Body2D::Body2D()
: id(generateRandomName()), mass(1), diameter(1), position(Vector2D()), velocity(Vector2D()), acceleration(Vector2D()), userObject(null)
{}

string Body2D::toString()
{
	return id + " ("+mass+"Kg, "+diameter+"Km)";
}

Vector2D Body2D::momentum() const
{
	return velocity.times(mass);
}

double Body2D::kineticEnergy() const
{
	return pow(velocity.magnitude(), 2) * mass * 0.5;
}
