/*
 * main_window.cpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#include "main_window.hpp"

TopWin* window;
void draw();

void showMainWindow()
{
	window = new TopWin("cplanets", Rect(0, 0, 640, 480), SDL_INIT_VIDEO, 0, draw);

	//start
	get_events();
}

void draw()
{
	window->clear();

	draw_title_ttf->draw_string(window->win, "Test", Point(20,40));
}
