/*
 * cli.hxx
 *
 *  Created on: 4 de out de 2016
 *      Author: carlosfaruolo
 */

void CPlanets::parse(int argc, char* argv[])
{
	// load program arguments
	for(int i = 1; i < argc; i++)
	{
		string argstr = string(argv[i]);

		if(argstr.empty()) continue;

		else if(argstr == "--centered" or argstr == "-c")
			SDL_util::preloadCentered();

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

