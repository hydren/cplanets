/*
 * main_window.hpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#ifndef MAIN_WINDOW_HPP_
#define MAIN_WINDOW_HPP_

#include "version.h"

//contains all cplanets gui related stuff
namespace CPlanets
{
	// parse command line arguments
	void parse(int argc, char* argv[]);

	// loads all assets and constructs the program window.
	void init();

	// show window and starts all its events treatments.
	void start();

	// returns a string containing help text
	const char* help();
}

#endif /* MAIN_WINDOW_HPP_ */
