/*
 * cli.hxx
 *
 *  Created on: 4 de out de 2016
 *      Author: carlosfaruolo
 */

#include "futil/math/parse_number.hpp"

const char* commandLineHelpTxt = "\n" \
	"Usage: cplanets [--version] [--help] [--centered] [--window-size <width> <height>] [--color-theme <name>] [--load-file <path>] \n" \
	"\n" \
	"Use --version or -v to display version. \n" \
	"Use --help or -h to show this help. \n" \
	"Use --centered or -c to start cplanets centered. \n" \
	"Use --window-size <width> <height> (or -w) to specify window size. \n" \
	"Use --hide-toolbars or -d to start cplanets with the toolbars hidden. \n" \
	"Use --color-theme <theme> (or -t) to specify a color theme. \n" \
	"Use --load-file <filePath> (or -l) to specify a file to open the program with. \n";

const char* CPlanets::help()
{
	return commandLineHelpTxt;
}

void CPlanets::parse(int argc, char* argv[])
{
	// load program arguments
	for(int i = 1; i < argc; i++)
	{
		string argstr = string(argv[i]);

		if(argstr.empty()) continue;

		else if(argstr == "--centered" or argstr == "-c")
			SDL_util::preloadCentered();

		else if(argstr == "--window-size" or argstr == "-w")
		{
			if(i + 2 < argc)
				if(parseable<unsigned>(argv[i+1]) and parseable<unsigned>(argv[i+2]))
				{
					windowSize.w = ::parse<unsigned>(string(argv[++i]));
					windowSize.h = ::parse<unsigned>(string(argv[++i]));
				}
				else cout << "Passed invalid arguments for " << argstr << ": \"" << argv[i+1] << "\" \"" << argv[i+2] << "\" (must be positive integers <width> <height>)" << endl;
			else cout << "Missing arguments for " << argstr << endl;
		}

		else if(argstr == "--hide-toobars" or argstr == "-d")
			aux_startToolbarHidden = true;

		else if(argstr == "--color-theme" or argstr == "-t")
		{
			if(i + 1 < argc) colorThemeName = string(argv[++i]);
			else cout << "Missing argument for " << argstr << endl;
		}

		else if(argstr == "--load-file" or argstr == "-l")
		{
			if(i + 1 < argc) filePathToLoad = string(argv[++i]);
			else cout << "Missing argument for " << argstr << endl;
		}

		else cout << "Unrecognized argument: " << argstr << endl;
	}
}

