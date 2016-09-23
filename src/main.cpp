/*
 * main.cpp
 *
 *  Created on: 02/03/2015
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>
 */

#include <cstdlib>
#include <ctime>

#include <string>
#include <iostream>
#include <stdexcept>

#include "physics/solvers/built_in.hpp"
#include "main_window.hpp"
#include "SDL_util.hpp"

#include "util.hpp"

using std::string;
using std::exception;
using std::cout;
using std::endl;

const string CPLANETS_VERSION = "0.9.3-beta";

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
	srand(time(null));

	for(int i = 1; i < argc; i++)
	{
		if(string(argv[i]) == "--version" or string(argv[i]) == "-v")
		{
			cout << CPLANETS_VERSION << endl;
			return EXIT_SUCCESS;
		}

		if(string(argv[i]) == "--help" or string(argv[i]) == "-h")
		{
			cout << "Use --version or -v to display version. \nUse --centered or -c to start cplanets centered. \nUse --help or -h to show this help." << endl;
			return EXIT_SUCCESS;
		}

		else if(string(argv[i]) == "--centered" or string(argv[i]) == "-c")
			SDL_util::preloadCentered();

		else cout << "Unrecognized argument: " << argv[i] << endl;
	}

	try
	{
		BuiltInSolvers::init();
		CPlanets::showMainWindow();
		return EXIT_SUCCESS;
	}
	catch (exception& e)
	{
		cout << "Application exited after uncaught exception: " << e.what() << endl;
		return EXIT_FAILURE;
	}
}
