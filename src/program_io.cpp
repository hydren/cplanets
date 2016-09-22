/*
 * program_io.cpp
 *
 *  Created on: 3 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "program_io.hpp"

#include <fstream>
#include <map>
#include <utility>

#include "futil/general/language.hpp"
#include "futil/string/more_operators.hpp"
#include "futil/string/actions.hpp"
#include "futil/string/split.hpp"
#include "futil/math/parse_number.hpp"

using std::string;
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
		if(ends_with(filename, "." + p.first))
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
		if(universe.gExp != 2.0) content = content + "\ngexp is " + universe.gExp;

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
		if(trim(string(buffer)) != "universe definition") return null; //invalid header

		// skip spaces
		do fis.getline(buffer, 512);
		while(fis.good() && trim(string(buffer)) == "");

		if(not starts_with(trim(string(buffer)), "gravity is ")) return null; //should gave gravity specified
		string gravityStr = split(trim(string(buffer)), ' ').back();
		if(not parseable<double>(gravityStr)) return null; //gravity value should be parseable
		double tmpGravity = parse<double>(gravityStr);

		// skip spaces
		do fis.getline(buffer, 512);
		while(fis.good() && trim(string(buffer)) == "");

		double tmpGExp = 2.0;
		bool isGExpSpecified = false;
		if(starts_with(trim(string(buffer)), "gexp is ")) //case gExp was specified
		{
			isGExpSpecified = true;
			string strGExp = split(trim(string(buffer)), ' ').back();
			if(not parseable<double>(strGExp)) //gExp value should be parseable (or not specified at all)
				return null;
			else
				tmpGExp = parse<double>(strGExp);
		}

		Universe2D* universe = new Universe2D();
		universe->gravity = tmpGravity;
		universe->gExp = tmpGExp;

		bool skipGetline = not isGExpSpecified;
		string line;
		while(fis.good())
		{
			if(not skipGetline)
				fis.getline(buffer, 512);
			else
				skipGetline = false;

			line = trim(string(buffer));
			if(line == "") continue; // skip spaces

			if(starts_with(trim(line), "body ") && contains(line, ":"))
			{
				vector<string> tokens = split(trim(line), ':');
				if(tokens.size() != 2) continue; //should have only one ':' separator

				string head = trim(tokens[0]);
				if(split(head, ' ').size() < 2) continue; //should have a id

				string id = split(head, ' ').back();

				tokens = split(tokens[1], ',');
				if(tokens.size() != 6) continue; //should have 6 named parameters

				if(not starts_with(trim(tokens[0]), "mass ")
				|| not starts_with(trim(tokens[1]), "diameter ")
				|| not starts_with(trim(tokens[2]), "position_x ")
				|| not starts_with(trim(tokens[3]), "position_y ")
				|| not starts_with(trim(tokens[4]), "velocity_x ")
				|| not starts_with(trim(tokens[5]), "velocity_y "))
					continue; //should have parameters named like these

				string token;
				token = split(trim(tokens[0]), ' ').back();
				if(not parseable<double>(token)) continue; //should have a parseable value
				double mass = parse<double>(token);

				token = split(trim(tokens[1]), ' ').back();
				if(not parseable<double>(token)) continue; //should have a parseable value
				double diameter = parse<double>(token);

				token = split(trim(tokens[2]), ' ').back();
				if(not parseable<double>(token)) continue; //should have a parseable value
				double position_x = parse<double>(token);

				token = split(trim(tokens[3]), ' ').back();
				if(not parseable<double>(token)) continue; //should have a parseable value
				double position_y = parse<double>(token);

				token = split(trim(tokens[4]), ' ').back();
				if(not parseable<double>(token)) continue; //should have a parseable value
				double velocity_x = parse<double>(token);

				token = split(trim(tokens[5]), ' ').back();
				if(not parseable<double>(token)) continue; //should have a parseable value
				double velocity_y = parse<double>(token);

				//add successfully read body
				universe->bodies.push_back(new Body2D(mass, diameter, Vector2D(position_x, position_y), Vector2D(velocity_x, velocity_y), Vector2D::NULL_VECTOR));
			}
		}

		return universe;
	}

	return null; //unknown format
}

