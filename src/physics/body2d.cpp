/*
 * body2d.cpp
 *
 *  Created on: 20/04/2015
 *      Author: felipe
 */

#include "body2d.hpp"

#include "futil/futil.hpp"

using std::string;

string generateRandomName()
{
	return string("B")+Math::randomBetween(0, 10000);
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
