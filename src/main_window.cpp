/*
 * main_window.cpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#include "main_window.hpp"

#include <cmath>

#include "SDL_widgets/SDL_widgets.h"

#include "planetarium.hpp"
#include "flow_layout.hpp"

// workaround to reroute output stream to console
FILE* workaround_sdl_stream_file = null;
void workaround_sdl_stream_file_close() // part of workaround
{
	if(workaround_sdl_stream_file != null) // if file var was used, close the file
		fclose(workaround_sdl_stream_file);
}
#ifdef _WIN32
	#ifdef HOTFIX_FOR_SDL_OUTPUT_STREAM_1
		void onSDLInit() // upon sdl init, reroute streams
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

using CPlanetsGUI::FlowLayoutPanel;

// ================ CONSTANTS ================
const unsigned TOOLBAR_SIZE = 32; // TOOLBAR_SIZE is used as size reference for buttons, spacing, etc
const unsigned WIDGETS_SPACING = 4;
const unsigned BODIES_PANEL_WIDTH = TOOLBAR_SIZE * 8;
const unsigned DEFAULT_VIEWPORT_TRANSLATE_RATE = 8;

//  ================ VARIABLES ===============

//  ================ COMPONENTS ================
TopWin* window; // The program window
Planetarium* planetarium;
Button* btnAddBody, *btnRecolorAll;
FlowLayoutPanel* toolbarNorth;

//  ================ FUNCTION PROTOTYPES ================
void draw(); // The drawing function.
void onWindowResize(int dw, int dh); // callback for window resizing events
void onKeyEvent(SDL_keysym *key,bool down);
void onButtonPressed(Button* btn);

//  ================ CPlanetsGUI namespace ================
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

void CPlanetsGUI::setComponentPosition(WinBase* component, Point& position)
{
	CPlanetsGUI::setComponentPosition(component, position.x, position.y);
}

void CPlanetsGUI::setComponentPosition(WinBase* component, int x, int y)
{
	CPlanetsGUI::setComponentPositionX(component, x);
	CPlanetsGUI::setComponentPositionY(component, y);
}

void CPlanetsGUI::setComponentPositionX(WinBase* component, int x)
{
	component->area.x = x;
	component->tw_area.x = x;
	component->title_area.x = x;
	component->title_top.x = x;
	if (component->parent)
	{
		component->tw_area.x=component->area.x+component->parent->tw_area.x;
	}
}

void CPlanetsGUI::setComponentPositionY(WinBase* component, int y)
{
	component->area.y = y;
	component->tw_area.y = y;
	component->title_area.y = y-17;
	component->title_top.y = y-17;
	if (component->parent)
	{
		component->tw_area.y=component->area.y+component->parent->tw_area.y;
	}
}

// ================ CPlanetsGUI::MainWindow namespace ================
void CPlanetsGUI::MainWindow::show()
{
	Rect windowSize(0, 0, 640, 480);
	window = new TopWin("cplanets", windowSize, SDL_INIT_VIDEO, SDL_RESIZABLE, draw, null, SDLMAIN_STREAM_WORKAROUND);
	handle_rsev = onWindowResize; //set callback for window resize
	handle_kev = onKeyEvent; //set callback for keyboard events

	Rect planetariumSize(
		BODIES_PANEL_WIDTH + WIDGETS_SPACING,
		TOOLBAR_SIZE + WIDGETS_SPACING,
		windowSize.w - BODIES_PANEL_WIDTH - TOOLBAR_SIZE - 2*WIDGETS_SPACING,
		windowSize.h - 2*TOOLBAR_SIZE
	);
	planetarium = new Planetarium(window, planetariumSize);

	Rect genericButtonSize(0, 0, TOOLBAR_SIZE, TOOLBAR_SIZE - 2*WIDGETS_SPACING);
	toolbarNorth = new FlowLayoutPanel(WIDGETS_SPACING, WIDGETS_SPACING);

	btnAddBody = new Button(window, 0, genericButtonSize, "Add", onButtonPressed);
	toolbarNorth->addComponent(btnAddBody);

	btnRecolorAll = new Button(window, 0, genericButtonSize, "Rec.", onButtonPressed);
	toolbarNorth->addComponent(btnRecolorAll);

	toolbarNorth->pack();

	//XXX DEBUG CODE START

	Body2D somebody(550, 32, Vector2D(64, 128), Vector2D(10, 0), Vector2D());
	somebody.userObject = getRandomColor();
	planetarium->physics->universe.bodies.push_back(somebody);
	planetarium->running = true;

	//XXX DEBUG CODE END

	//start
	get_events();
	workaround_sdl_stream_file_close(); // part of workaround
}

//  ================ CALLBACK DEFINITIONS ================
void draw()
{
	window->clear();
	const char* versionStr = ("v"+CPLANETS_VERSION).c_str();
	draw_title_ttf->draw_string(window->win, versionStr, Point(window->tw_area.w - draw_title_ttf->text_width(versionStr), 4));
}

void onWindowResize(int dw, int dh)
{
	planetarium->widen(dw, dh);
}

void onKeyEvent(SDL_keysym *key,bool down)
{
	switch (key->sym) {
		case SDLK_UP:
			planetarium->currentViewportTranlationRate.y = down? -DEFAULT_VIEWPORT_TRANSLATE_RATE : 0;
			break;
		case SDLK_DOWN:
			planetarium->currentViewportTranlationRate.y = down?  DEFAULT_VIEWPORT_TRANSLATE_RATE : 0;
			break;
		case SDLK_LEFT:
			planetarium->currentViewportTranlationRate.x = down? -DEFAULT_VIEWPORT_TRANSLATE_RATE : 0;
			break;
		case SDLK_RIGHT:
			planetarium->currentViewportTranlationRate.x = down?  DEFAULT_VIEWPORT_TRANSLATE_RATE : 0;
			break;
		default:
			break;
	}
}

void onButtonPressed(Button* btn)
{
	if(btn == btnAddBody)
	{
		Body2D somebody(550, 32, Vector2D(randomBetween(32, 64), randomBetween(64, 128)), Vector2D(randomBetween(0, 8), randomBetween(0, 8)), Vector2D());
		somebody.userObject = CPlanetsGUI::getRandomColor();
		planetarium->physics->universe.bodies.push_back(somebody);
	}
}
