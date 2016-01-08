/*
 * main.cpp
 *
 *  Created on: 02/03/2015
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>
 */

#include <cstdlib>

#include "geometry/vector2d.hpp"
#include "main_window.hpp"

#ifdef _WIN32
	#define SDLMAIN_STREAM_WORKAROUND workaround_sdl_stream_file = fopen("CON", "w" ); freopen( "CON", "w", stdout ); freopen( "CON", "w", stderr )
#else
	#define SDLMAIN_STREAM_WORKAROUND ;
#endif

FILE* workaround_sdl_stream_file = NULL;// part of workaround

int main(int arc, char* argv[])
{
  SDLMAIN_STREAM_WORKAROUND;// part of workaround
  puts("Program start");
  showMainWindow();
  if(workaround_sdl_stream_file != NULL) fclose(workaround_sdl_stream_file); // part of workaround
  return EXIT_SUCCESS;
}






