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
#include "main_window.hpp"

const string CPLANETS_VERSION = "0.1.1-alpha";

int main(int arc, char* argv[])
{
	try
	{
		srand(time(null));
		CPlanetsGUI::MainWindow::show();
		return EXIT_SUCCESS;
	}
	catch (std::exception& e)
	{
		std::cout << "application exited after uncaught exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
