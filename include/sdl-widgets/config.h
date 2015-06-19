/*
 * config.h
 *
 *  Created on: 21/04/2015
 *      Author: felipe
 */

#ifndef SDL_WIDGETS_CONFIG_H_
#define SDL_WIDGETS_CONFIG_H_

//Replace this for a proper configuration file (or remove it altogether)

#ifdef _WIN32
const char* FONTPATH="C:\\Windows\\Fonts\\DejaVuSans.ttf";
const char* FONTPATH_BOLD="C:\\Windows\\Fonts\\DejaVuSans-Bold.ttf";
const char* FONTPATH_MONO="C:\\Windows\\Fonts\\DejaVuSans-Bold.ttf";
#endif

#ifndef _WIN32

const char* FONTPATH="/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf";
const char* FONTPATH_BOLD="/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans-Bold.ttf";
const char* FONTPATH_MONO="/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans-Bold.ttf";

#endif

#endif /* SDL_WIDGETS_CONFIG_H_ */
