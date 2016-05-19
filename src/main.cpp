/*
 * main.cpp
 *
 *  Created on: 02/03/2015
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>
 */

#include <cstdlib>
#include <ctime>

#include <stdexcept>
#include <iostream>

#include "geometry/vector2d.hpp"

#include "util.hpp"
#include "main_window.hpp"

#include <string>
using std::string;
using std::exception;
using std::cout; using std::endl;

const string CPLANETS_VERSION = "0.5.1-beta";

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

int main(int arc, char* argv[])
{
	try
	{
		srand(time(null));
//		putenv("SDL_VIDEO_CENTERED=1"); // todo use video centering when able to choose on settings.
		CPlanets::showMainWindow();
		return EXIT_SUCCESS;
	}
	catch (exception& e)
	{
		cout << "application exited after uncaught exception: " << e.what() << endl;
		return EXIT_FAILURE;
	}
}
