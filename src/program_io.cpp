/*
 * program_io.cpp
 *
 *  Created on: 3 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "program_io.hpp"

#include <map>
#include <utility>

#include "futil/futil.hpp"

using std::map;
using std::vector;
using std::pair;
using ApplicationIO::FileFormat;

map<string, FileFormat> formatOf;
map<FileFormat, vector<string> > extensionOf;

void put(const char* ext, ApplicationIO::FileFormat ff)
{
	formatOf[ext] = ff;
	extensionOf[ff].push_back(ext);
}

void init()
{
	static bool alreadyInit = false; if(alreadyInit) return;

	put("txt", ApplicationIO::FORMAT_TXT);
	//...

	alreadyInit = true;
}

FileFormat inferFormat(const string& filename)
{
	init();
	typedef pair<string, FileFormat> Pair;
	typedef map<string, FileFormat> Map;
	foreach(Pair, p, Map, formatOf)
	{
		if(String::endsWith(filename, "." + p.first))
		{
			return p.second;
		}
	}
	return ApplicationIO::FORMAT_DEFAULT;
}

void ApplicationIO::save(const Universe2D& universe, const std::string& filename) //will guess format from extension (if recognized, else FORMAT_DEFAULT)
{
	init();
	save(universe, filename, inferFormat(filename), false);
}

void ApplicationIO::save(const Universe2D& universe, const std::string& filename, bool appendExtension)
{
	save(universe, filename, FORMAT_DEFAULT, appendExtension);
}

void ApplicationIO::save(const Universe2D& universe, const std::string& filename, FileFormat format)
{
	save(universe, filename, format, true);
}

void ApplicationIO::save(const Universe2D& universe, const std::string& barefilename, FileFormat format, bool appendExtension)
{
	init();
	if(format == FORMAT_DEFAULT)
		format = FORMAT_TXT;

	string filename = (appendExtension? barefilename + extensionOf[format][0] : barefilename);

	if(format == FORMAT_TXT)
	{
		string content = "universe definition";
		content = content + "\ngravity is " + universe.gravity;

		const_foreach(const Body2D*, body, vector<Body2D*>, universe.bodies)
		{
			content = content + "\nbody " + body->id + ":";
			content = content +  " mass " + body->mass;
			content = content + ", diameter " + body->diameter;
			content = content + ", position_x " + body->position.x;
			content = content + ", position_y " + body->position.y;
			content = content + ", velocity_x " + body->velocity.x;
			content = content + ", velocity_y " + body->velocity.y;
		}
		FileOutputStream fos(filename.c_str(), FileOutputStream::out);
		fos << content;
		fos.close();
		return;
	}

	//todo implement other formats to save...
}

Universe2D* ApplicationIO::load(std::string filename)
{
	init();
	return load(filename, inferFormat(filename));
}

Universe2D* ApplicationIO::load(std::string filename, FileFormat format)
{
	init();
	if(format == FORMAT_DEFAULT)
		format = FORMAT_TXT;

	if(format == FORMAT_TXT)
	{
		char buffer[512];
		FileInputStream fis(filename.c_str());
		fis.getline(buffer, 512);
		if(String::trim(string(buffer)) != "universe definition") return null; //invalid header

		// skip spaces
		do fis.getline(buffer, 512);
		while(fis.good() && String::trim(string(buffer)) == "");

		if(not String::startsWith(String::trim(string(buffer)), "gravity is ")) return null; //should gave gravity specified

		string gravityStr = String::split(String::trim(string(buffer)), ' ').back();

		if(not String::parseable<double>(gravityStr)) return null; //gravity value should be parseable

		Universe2D* universe = new Universe2D();
		universe->gravity = String::parse<double>(gravityStr);

		string line;
		while(fis.good())
		{
			fis.getline(buffer, 512);
			line = String::trim(string(buffer));

			if(line == "") continue; // skip spaces

			if(String::startsWith(String::trim(line), "body ") && String::contains(line, ":"))
			{
				vector<string> tokens = String::split(String::trim(line), ':');
				if(tokens.size() != 2) continue; //should have only one ':' separator

				string head = String::trim(tokens[0]);
				if(String::split(head, ' ').size() < 2) continue; //should have a id

				string id = String::split(head, ' ').back();

				tokens = String::split(tokens[1], ',');
				if(tokens.size() != 6) continue; //should have 6 named parameters

				if(not String::startsWith(String::trim(tokens[0]), "mass ")
				|| not String::startsWith(String::trim(tokens[1]), "diameter ")
				|| not String::startsWith(String::trim(tokens[2]), "position_x ")
				|| not String::startsWith(String::trim(tokens[3]), "position_y ")
				|| not String::startsWith(String::trim(tokens[4]), "velocity_x ")
				|| not String::startsWith(String::trim(tokens[5]), "velocity_y "))
					continue; //should have parameters named like these

				string token;
				token = String::split(String::trim(tokens[0]), ' ').back();
				if(not String::parseable<double>(token)) continue; //should have a parseable value
				double mass = String::parse<double>(token);

				token = String::split(String::trim(tokens[1]), ' ').back();
				if(not String::parseable<double>(token)) continue; //should have a parseable value
				double diameter = String::parse<double>(token);

				token = String::split(String::trim(tokens[2]), ' ').back();
				if(not String::parseable<double>(token)) continue; //should have a parseable value
				double position_x = String::parse<double>(token);

				token = String::split(String::trim(tokens[3]), ' ').back();
				if(not String::parseable<double>(token)) continue; //should have a parseable value
				double position_y = String::parse<double>(token);

				token = String::split(String::trim(tokens[4]), ' ').back();
				if(not String::parseable<double>(token)) continue; //should have a parseable value
				double velocity_x = String::parse<double>(token);

				token = String::split(String::trim(tokens[5]), ' ').back();
				if(not String::parseable<double>(token)) continue; //should have a parseable value
				double velocity_y = String::parse<double>(token);

				//add successfully read body
				universe->bodies.push_back(new Body2D(mass, diameter, Vector2D(position_x, position_y), Vector2D(velocity_x, velocity_y), Vector2D::NULL_VECTOR));
			}
		}

		return universe;
	}

	return null; //unknown format
}

