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

#include <vector>
#include <algorithm>

using std::vector;
using std::string;

struct id_generator
{
	vector<unsigned> available_ids;
	unsigned last, size;

	id_generator(unsigned size=10000)
	: available_ids(), last(0), size(size)
	{ refill(); }

	void refill()
	{
		for(unsigned i = last; i < size; i++)
			available_ids.push_back(i);

		std::random_shuffle(available_ids.begin(), available_ids.end());
		last = size;
		size *= 2;
	}

	string generateRandomName()
	{
		if(available_ids.empty())
			refill();

		string result = string("B") + available_ids.back();
		available_ids.pop_back();
		return result;
	}
};

id_generator gen;

Body2D::Body2D(double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration)
: mass(mass), diameter(diameter), position(position), velocity(velocity), acceleration(acceleration), userObject(null)
{
	id = gen.generateRandomName();
}

Body2D::Body2D(string id, double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration)
: id(id), mass(mass), diameter(diameter), position(position), velocity(velocity), acceleration(acceleration), userObject(null)
{}

Body2D::Body2D()
: id(gen.generateRandomName()), mass(1), diameter(1), position(Vector2D()), velocity(Vector2D()), acceleration(Vector2D()), userObject(null)
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
