/*
 * main.cpp
 *
 *  Created on: 02/03/2015
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>
 */

#include <ciso646>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <string>
#include <iostream>
#include <stdexcept>

#include "physics/solvers/built_in.hpp"
#include "main_window.hpp"

#include "util.hpp"

using std::string;
using std::exception;
using std::cout;
using std::endl;

const string CPLANETS_VERSION = "0.9.4-beta";

const string CPLANETS_LICENSE =
		"Copyright (C) 2016 Carlos F. M. Faruolo\n"
		"E-mail: 5carlosfelipe5@gmail.com\n\n"

		"This program is free software: you can redistribute it and/or modify "
		"it under the terms of the GNU General Public License as published by "
		"the Free Software Foundation, either version 3 of the License, or "
		"(at your option) any later version.\n\n"

		"This program is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
		"GNU General Public License for more details.\n "

		"You should have received a copy of the GNU General Public License "
		"along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n";

int main(int argc, char* argv[])
{
	for(int i = 1; i < argc; i++)
	if(strncmp(argv[i], "--version", 10) == 0 or strncmp(argv[i], "-v", 3) == 0)
	{
		cout << CPLANETS_VERSION << endl;
		return EXIT_SUCCESS;
	}

	srand(time(null));

	try
	{
		BuiltInSolvers::init();
		CPlanets::parseCmdArgs(argc, argv);
		CPlanets::init();
		CPlanets::start();
		return EXIT_SUCCESS;
	}
	catch (exception& e)
	{
		cout << "Application exited after uncaught exception: " << e.what() << endl;
		return EXIT_FAILURE;
	}
}
