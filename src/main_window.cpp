/*
 * main_window.cpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#include "main_window.hpp"

#include <cmath>

#include "planetarium.hpp"

const string VERSION_STRING = ("v"+CPLANETS_VERSION);

const int TOOLBAR_SIZE = 24;
const int WIDGETS_SPACING = 4;
const int BODIES_PANEL_WIDTH = TOOLBAR_SIZE * 8;

TopWin* window;
Planetarium* planetarium;

void draw();
void onWindowResize(int dw, int dh);

// workaround to reroute output stream to console
FILE* workaround_sdl_stream_file = null;
#ifdef _WIN32
	#ifdef HOTFIX_FOR_SDL_OUTPUT_STREAM_1
		void onSDLInit()
		{
			workaround_sdl_stream_file = fopen("CON", "w" );
			freopen( "CON", "w", stdout );
			freopen( "CON", "w", stderr );
		}
		#define SDLMAIN_STREAM_WORKAROUND onSDLInit
	#else
		#define SDLMAIN_STREAM_WORKAROUND null
	#endif
#else
	#define SDLMAIN_STREAM_WORKAROUND null
#endif

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
	Rect winSize(0, 0, 640, 480);
	Rect planetariumSize(
		BODIES_PANEL_WIDTH + WIDGETS_SPACING,
		TOOLBAR_SIZE + WIDGETS_SPACING,
		winSize.w - BODIES_PANEL_WIDTH - TOOLBAR_SIZE - 2*WIDGETS_SPACING,
		winSize.h - 2*TOOLBAR_SIZE
	);
	window = new TopWin("cplanets", winSize, SDL_INIT_VIDEO, SDL_RESIZABLE, draw, null, SDLMAIN_STREAM_WORKAROUND);
	planetarium = new Planetarium(window, planetariumSize);
	handle_rsev = onWindowResize;

	//XXX DEBUG CODE START

	Body2D somebody(550, 32, Vector2D(64, 128), Vector2D(10, 0), Vector2D());
	somebody.userObject = getRandomColor();
	planetarium->physics->universe.bodies.push_back(somebody);
	planetarium->running = true;

	//XXX DEBUG CODE END

	//start
	get_events();
	if(workaround_sdl_stream_file != null) fclose(workaround_sdl_stream_file); // part of workaround
}

void onWindowResize(int dw, int dh)
{
	planetarium->widen(dw, dh);
}

void draw()
{
	window->clear();
	draw_title_ttf->draw_string(window->win, VERSION_STRING.c_str(), Point(window->tw_area.w - draw_title_ttf->text_width(VERSION_STRING.c_str()), 4));
}
