/*
 * main_window.hpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#ifndef MAIN_WINDOW_HPP_
#define MAIN_WINDOW_HPP_

#include "util.hpp"

extern const unsigned WIDGETS_SPACING;

//contains all cplanets gui related stuff
namespace CPlanets
{
	// parse command line arguments
	void parse(int argc, char* argv[]);

	// loads all assets and constructs the program window.
	void init();

	// loads current file data to start the program with.
	void load(const std::string& filePath);

	// show window and starts all its events treatments.
	void start();
}

#endif /* MAIN_WINDOW_HPP_ */
