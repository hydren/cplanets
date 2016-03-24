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
void onWindowResize(int dw, int dh);

int CPlanetsGUI::colorToInt(const SDL_Surface* surf, const SDL_Color& color, bool forceRGBA)
{
	#ifdef HOTFIX_FOR_SDL_MAP_RGB_1
	if(forceRGBA || surf == null)
		return (((int) color.r) << 24) + (((int) color.g) << 16) + (((int) color.b) << 8) + 0x000000FF;

	#endif

	return SDL_MapRGB(surf->format, color.r, color.g, color.b);
}

void CPlanetsGUI::modifyColor(SDL_Color& color, int r, int g, int b)
{
	if(r != -1) color.r = r;
	if(b != -1) color.b = b;
	if(g != -1) color.g = g;
}

SDL_Color* CPlanetsGUI::getRandomColor()
{
	SDL_Color* somecolor = new SDL_Color;
	somecolor->r = randomBetween(0, 255);
	somecolor->g = randomBetween(0, 255);
	somecolor->b = randomBetween(0, 255);
	return somecolor;
}

void CPlanetsGUI::triggerRepaint()
{
	SDL_Event repaintEvent;
	repaintEvent.type = SDL_VIDEOEXPOSE;
	SDL_PushEvent(&repaintEvent);
}


void CPlanetsGUI::MainWindow::show()
{
	window = new TopWin("cplanets", Rect(0, 0, 640, 480), SDL_INIT_VIDEO, SDL_RESIZABLE, draw);
	planetarium = new Planetarium(window, Rect(128, 64, 400, 300));
	handle_rsev = onWindowResize;

	//XXX DEBUG CODE START

	Body2D somebody(550, 32, Vector2D(64, 128), Vector2D(10, 0), Vector2D());
	somebody.userObject = getRandomColor();
	planetarium->physics->universe.bodies.push_back(somebody);
	planetarium->running = true;

	//XXX DEBUG CODE END

	//start
	get_events();
}

void onWindowResize(int dw, int dh)
{
	//XXX dummy resizing code, for testing purposes
	planetarium->widen(dw * 0.75, dh * 0.75);
}

void draw()
{
	window->clear();
	draw_title_ttf->draw_string(window->win, "Test", Point(20,40));
}
