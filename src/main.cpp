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

const string CPLANETS_VERSION = "0.3.7-beta";

int main(int arc, char* argv[])
{
	try
	{
		srand(time(null));
		CPlanetsGUI::MainWindow::show();
		return EXIT_SUCCESS;
	}
	catch (exception& e)
	{
		cout << "application exited after uncaught exception: " << e.what() << endl;
		return EXIT_FAILURE;
	}
}
