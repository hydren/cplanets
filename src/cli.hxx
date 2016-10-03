/*
 * cli.hxx
 *
 *  Created on: 3 de out de 2016
 *      Author: carlosfaruolo
 */

#include <ciso646>
#include <tclap/CmdLine.h>

using TCLAP::CmdLine;
using TCLAP::SwitchArg;
using TCLAP::ValueArg;

void CPlanets::parseCmdArgs(int argc, char* argv[])
{
	CmdLine cmd("cplanets", ' ', CPLANETS_VERSION, true);

	SwitchArg triggerCentered("c", "centered", "When set, attempts to start the program positioned on the center. Default is false.", false);
	cmd.add(triggerCentered);

	ValueArg<string> argColorTheme("t", "color-theme", "Specify which color theme to use. If the theme could not be recognized, the default is used.", false, "default", "string");
	cmd.add(argColorTheme);

	cmd.parse(argc, argv);

	if(triggerCentered.isSet())
		SDL_util::preloadCentered();

	if(argColorTheme.isSet())
		colorThemeName = argColorTheme.getValue();
}
