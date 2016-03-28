/*
 * hotfixes.h
 *
 *  Created on: 21 de mar de 2016
 *      Author: carlosfaruolo
 */

#ifndef HOTFIXES_H_
#define HOTFIXES_H_

//Comment this hotfix if the SDL_MapRBG function is not bugged in the SDL version you're using to compile
#define HOTFIX_FOR_SDL_MAP_RGB_1

//Comment this to disable rerouting the output stream to the console and leave the stream on stdout.txt and stderr.txt
#define HOTFIX_FOR_SDL_OUTPUT_STREAM_1

#endif /* HOTFIXES_H_ */
