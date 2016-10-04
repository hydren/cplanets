/*
 * main.cpp
 *
 *  Created on: 02/03/2015
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>
 */

#include <cstdlib>
#include <ctime>
#include <cstring>

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
	// quick execution modes
	for(int i = 1; i < argc; i++)
	if(strncmp(argv[i], "--version", 10) == 0 or strncmp(argv[i], "-v", 3) == 0)
	{
		cout << CPLANETS_VERSION << endl;
		return EXIT_SUCCESS;
	}
	else if(strncmp(argv[i], "--help", 7) == 0 or strncmp(argv[i], "-h", 3) == 0)
	{
		cout << "Use --version or -v to display version." << '\n';
		cout << "Use --centered or -c to start cplanets centered." << '\n';
		cout << "Use --help or -h to show this help." << '\n';
		cout << endl;
		return EXIT_SUCCESS;
	}

	string colorTheme = "default";

	// load program arguments
	for(int i = 1; i < argc; i++)
	{
		string argstr = string(argv[i]);

		if(argstr.empty()) continue;

		else if(argstr == "--centered" or argstr == "-c")
			SDL_util::preloadCentered();

		else if(argstr == "--color-theme" or argstr == "-t")
		{
			if(i + 1 < argc)
			{
				colorTheme = string(argv[i+1]);
				i++;
			}
			else
			{
				cout << "Missing argument for " << argstr << endl;
			}
		}

		else cout << "Unrecognized argument: " << argstr << endl;
	}

	srand(time(null));

	try
	{
		BuiltInSolvers::init();
		CPlanets::init(colorTheme);
		CPlanets::start();
		return EXIT_SUCCESS;
	}
	catch (exception& e)
	{
		cout << "Application exited after uncaught exception: " << e.what() << endl;
		return EXIT_FAILURE;
	}
}
