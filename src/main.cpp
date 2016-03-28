/*
 * main.cpp
 *
 *  Created on: 02/03/2015
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>
 */

#include <cstdlib>

#include "geometry/vector2d.hpp"
//#include "sdl-widgets/example.hpp"
#include "main_window.hpp"
#include <ctime>

const string CPLANETS_VERSION = "0.1.1-alpha";

int main(int arc, char* argv[])
{
	srand(time(null));
	CPlanetsGUI::MainWindow::show();
	//Widgets_RunExample();
	return EXIT_SUCCESS;
}
