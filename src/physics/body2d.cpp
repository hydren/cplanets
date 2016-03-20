/*
 * body2d.cpp
 *
 *  Created on: 20/04/2015
 *      Author: felipe
 */

#include "body2d.hpp"

string generateRandomName()
{
	return string("B")+randomBetween(0, 10000);
}

Body2D::Body2D(double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration)
: mass(mass), diameter(diameter), position(position), velocity(velocity), acceleration(acceleration), userObject(NULL)
{
	id = generateRandomName();
}

Body2D::Body2D(string id, double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration)
: id(id), mass(mass), diameter(diameter), position(position), velocity(velocity), acceleration(acceleration), userObject(NULL)
{}

Body2D::Body2D()
: id(generateRandomName()), mass(1), diameter(1), position(Vector2D()), velocity(Vector2D()), acceleration(Vector2D()), userObject(NULL)
{}

/** Copy constructor (deep). Creates a new Body2D instance with the same properties (including ID). */
Body2D::Body2D(const Body2D& b)
: id(b.id), mass(b.mass), diameter(b.diameter), position(b.position), velocity(b.velocity), acceleration(b.acceleration), userObject(b.userObject)
{}

string Body2D::toString()
{
	return id + " ("+mass+"Kg, "+diameter+"Km)";
}
