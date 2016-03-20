/*
 * main_window.cpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#include "main_window.hpp"
#include "planetarium.hpp"

TopWin* window;
Planetarium* planetarium;

void draw();

int CPlanetsGUI::colorToInt(const SDL_Color& color)
{
	return SDL_MapRGB(window->win->format, color.r, color.g, color.b);
}

void CPlanetsGUI::modifyColor(SDL_Color& color, int r, int g, int b)
{
	if(r != -1) color.r = r;
	if(b != -1) color.b = b;
	if(g != -1) color.g = g;
}

void CPlanetsGUI::MainWindow::show()
{
	window = new TopWin("cplanets", Rect(0, 0, 640, 480), SDL_INIT_VIDEO, SDL_RESIZABLE, draw);
	planetarium = new Planetarium(window, Rect(128, 64, 400, 300));

	//start
	get_events();
}

void draw()
{
	window->clear();
	draw_title_ttf->draw_string(window->win, "Test", Point(20,40));
	planetarium->draw();
}
