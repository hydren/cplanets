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

map<string, ApplicationIO::FileFormat> formatOf;
map<ApplicationIO::FileFormat, vector<string> > extensionOf;

void put(const char* ext, ApplicationIO::FileFormat ff)
{
	formatOf[ext] == ff;
	extensionOf[ff].push_back(ext);
}

void init()
{
	static bool alreadyInit = false; if(alreadyInit) return;

	put("txt", ApplicationIO::FORMAT_TXT);
	//...

	alreadyInit = true;
}

void ApplicationIO::save(const Universe2D& universe, const std::string& filename) //will guess format from extension (if recognized, else FORMAT_DEFAULT)
{
	init();
	typedef pair<string, FileFormat> Pair;
	typedef map<string, FileFormat> Map;
	foreach(Pair, p, Map, formatOf)
	{
		if(String::endsWith(filename, p.first))
		{
			save(universe, filename, p.second, false);
			return;
		}
	}
	save(universe, filename, FORMAT_DEFAULT, false);
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
			content = content + ", position " + body->position.toString();
			content = content + ", velocity " + body->velocity.toString();
			content = content + ", acceleration " + body->acceleration.toString();
			FileOutputStream fos(filename.c_str(), FileOutputStream::out);
			fos << content;
			fos.close();
		}

		return;
	}

	//todo implement other formats to save...
}


Universe2D* ApplicationIO::load(std::string filename, FileFormat ff)
{
	init();
	return null;
}

