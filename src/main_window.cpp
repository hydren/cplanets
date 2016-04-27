/*
 * main_window.cpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#include "main_window.hpp"

#include <cmath>
#include <cstring>

#include "SDL_widgets/SDL_widgets.h"
#include "futil/futil.hpp"
#include "SDL_util.hpp"

#include "planetarium.hpp"
#include "widgets/flow_layout.hpp"
#include "widgets/drop_menu.hpp"
#include "widgets/spinner.hpp"
#include "widgets/widgets_util.hpp"

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

using std::cout; using std::endl;
using std::vector;
using CPlanetsGUI::FlowLayout;
using Math::randomBetween;
using SDL_util::LabeledComponentPacker;
using SDL_util::packLabeledComponent;

void runOnce(void(func)(void))
{
	static bool once = false;
	if(not once) { func(); once = true; }
}

// ================ CONSTANTS ================
const unsigned TOOLBAR_SIZE = 32; // TOOLBAR_SIZE is used as size reference for buttons, spacing, etc
const unsigned WIDGETS_SPACING = 4;
const unsigned BODIES_PANEL_WIDTH = TOOLBAR_SIZE * 7;
const int PLANETARIUM_ID = 959;
string VERSION_TEXT;

//  ================ VARIABLES ===============

//  ================ COMPONENTS ================
TopWin* window; // The program window
Planetarium* planetarium;
FlowLayout* toolbarNorthLayout, *toolbarSouthLayout;
Button* btnAddBody, *btnAddRandom, *btnRecolorAll, *btnRun, *btnPause;
CheckBox* chckTraceOrbit;
Spinner<unsigned>* spnTraceLength;
Spinner<double>* spnBodyDiameter, *spnBodyDensity;
DropDownMenu* ddmTraceStyle;
TextWin* txtBodies;
RExtButton* rbBodies, *rbOptions;
ExtRButCtrl* rbcTabControl;

Rect genericButtonSize(0, 0, TOOLBAR_SIZE, TOOLBAR_SIZE);

//  ================ FUNCTION PROTOTYPES ================
void draw(); // The drawing function.
void onWindowResize(int dw, int dh); // callback for window resizing events
void onKeyEvent(SDL_keysym *key,bool down);
void onButtonPressed(Button* btn);
void onCheckBoxPressed(CheckBox* chck);
void onCheckBoxPressed(CheckBox* chck, bool fake);
void onDropDownMenuButton(RButWin*,int nr,int fire);
void onTabPressed(RExtButton*,bool is_act);
void onUserEvent(int cmd,int param,int param2);
void onPlanetariumBodyCollision(vector<Body2D>& collidingList, Body2D& resultingMerger);
void onPlanetariumBodyCreation(Body2D& createdBody);
void onReady();

//  ==================== PLANETARIUM LISTENER ===========================

struct CustomUniverseListener extends Planetarium::UniverseEventListener
{
	void onBodyCollision(vector<Body2D>& collidingList, Body2D& resultingMerger) { onPlanetariumBodyCollision(collidingList, resultingMerger); }
	void onBodyCreation(Body2D& createdBody) { onPlanetariumBodyCreation(createdBody); }
};

// ================ CPlanetsGUI::MainWindow namespace ================
void CPlanetsGUI::MainWindow::show()
{
	Rect windowSize(0, 0, 640, 480);
	window = new TopWin("cplanets", windowSize, SDL_INIT_VIDEO, SDL_RESIZABLE, draw, null, SDLMAIN_STREAM_WORKAROUND);
	handle_rsev = onWindowResize; //set callback for window resize
	handle_kev = onKeyEvent; //set callback for keyboard events
	handle_uev = onUserEvent;
	VERSION_TEXT ="v"+CPLANETS_VERSION;

	Rect planetariumSize(
		BODIES_PANEL_WIDTH + WIDGETS_SPACING,
		TOOLBAR_SIZE + WIDGETS_SPACING,
		windowSize.w - (BODIES_PANEL_WIDTH + TOOLBAR_SIZE + 2*WIDGETS_SPACING),
		windowSize.h - (2.25*TOOLBAR_SIZE)
	);
	planetarium = new Planetarium(window, planetariumSize, PLANETARIUM_ID);
	planetarium->addUniverseEventListener(new CustomUniverseListener());

	toolbarNorthLayout = new FlowLayout(WIDGETS_SPACING, WIDGETS_SPACING);

	LabeledComponentPacker packer(TOOLBAR_SIZE-2*WIDGETS_SPACING);

	btnAddBody = new Button(window, 0, genericButtonSize, "Add", onButtonPressed);
	packer.pack(btnAddBody);
	toolbarNorthLayout->addComponent(btnAddBody);

	btnAddRandom = new Button(window, 0, genericButtonSize, "Add random", onButtonPressed);
	packer.pack(btnAddRandom);
	toolbarNorthLayout->addComponent(btnAddRandom);


	btnRecolorAll = new Button(window, 0, genericButtonSize, "Recolor all bodies", onButtonPressed);
	packer.pack(btnRecolorAll);
	toolbarNorthLayout->addComponent(btnRecolorAll);

	toolbarNorthLayout->addComponent(new CPlanetsGUI::Layout::Spacer(toolbarNorthLayout));

	btnRun = new Button(window, 0, genericButtonSize, "Run", onButtonPressed);
	packer.pack(btnRun);
	toolbarNorthLayout->addComponent(btnRun);

	btnPause = new Button(window, 0, genericButtonSize, "Pause", onButtonPressed);
	packer.pack(btnPause);
	toolbarNorthLayout->addComponent(btnPause);

	toolbarNorthLayout->pack();

	Rect txtBodiesSize(
		WIDGETS_SPACING,
		TOOLBAR_SIZE + WIDGETS_SPACING + TTF_FontHeight(draw_title_ttf->ttf_font),
		BODIES_PANEL_WIDTH - WIDGETS_SPACING,
		windowSize.h - 2.25*TOOLBAR_SIZE - TTF_FontHeight(draw_title_ttf->ttf_font)
	);
	txtBodies = new TextWin(window, 0, txtBodiesSize, 8, "Bodies");

	toolbarSouthLayout = new FlowLayout(WIDGETS_SPACING, windowSize.h - (1.25*TOOLBAR_SIZE - 2*WIDGETS_SPACING));
	toolbarSouthLayout->alignment = FlowLayout::MIDDLE;

	chckTraceOrbit = new CheckBox(window, 0, genericButtonSize, "Show orbit trace", onCheckBoxPressed);
	chckTraceOrbit->d = &(planetarium->orbitTracer.isActive);  // binds the checkbox to the variable
	packLabeledComponent(chckTraceOrbit);
	toolbarSouthLayout->addComponent(chckTraceOrbit);

	spnTraceLength = new Spinner<unsigned>(window, Rect(0, 0, 3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Trace length:");
	spnTraceLength->setValue(&(planetarium->orbitTracer.traceLength));
	spnTraceLength->offset.y -= 2;
	toolbarSouthLayout->addComponent(static_cast<CPlanetsGUI::Layout::Element*>(spnTraceLength));

	DropDownMenuFactory factory;
	factory.setLabel("Trace style: ", true);
	factory.setAppearance(DropDownMenuFactory::COMBOBOX);
	factory.setSize(Rect(40, 40, 100, 20));
	factory.addItem("Linear");
	factory.addItem("Point");
	factory.setCallback(onDropDownMenuButton);
	ddmTraceStyle = factory.createAt(window);
	ddmTraceStyle->offset.y = -10;
	toolbarSouthLayout->addComponent(ddmTraceStyle);

	toolbarSouthLayout->addComponent(new Layout::Spacer(Rect(0,0,8,0)));

	spnBodyDiameter = new Spinner<double>(window, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Diameter:");
	spnBodyDiameter->setValue(&(planetarium->bodyCreationDiameterRatio));
	spnBodyDiameter->setStepValue(0.1);
	spnBodyDiameter->offset.y -= 2;
	toolbarSouthLayout->addComponent(static_cast<CPlanetsGUI::Layout::Element*>(spnBodyDiameter));

	spnBodyDensity = new Spinner<double>(window, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Density:");
	spnBodyDensity->setValue(&(planetarium->bodyCreationDensity));
	spnBodyDensity->setStepValue(0.1);
	spnBodyDensity->offset.y -= 2;
	toolbarSouthLayout->addComponent(static_cast<CPlanetsGUI::Layout::Element*>(spnBodyDensity));

	toolbarSouthLayout->pack();

//	FlowLayout tabLayout(WIDGETS_SPACING, TOOLBAR_SIZE + WIDGETS_SPACING);
//	tabLayout.spacing_h = 1;
//	rbcTabControl = new ExtRButCtrl(Style(0, calc_color(0xe0e0e0)), onTabPressed);
//	rbcTabControl->maybe_z = false;
//
//	rbBodies = rbcTabControl->add_extrbut(window, Rect(0,0,1,1), "Bodies", 1);
//	packLabeledComponent(rbBodies);
//	tabLayout.addComponent(rbBodies);
//
//	rbOptions = rbcTabControl->add_extrbut(window, Rect(0,0,1,1), "Options", 1);
//	packLabeledComponent(rbOptions);
//	tabLayout.addComponent(rbOptions);
//
//	tabLayout.pack();

	//start
	planetarium->setRunning();
	get_events();
	workaround_sdl_stream_file_close(); // part of workaround
}

//  ================ CALLBACK DEFINITIONS ================
void draw()
{
	window->clear();
	Point versionStringPos(window->tw_area.w - WIDGETS_SPACING - draw_title_ttf->text_width(VERSION_TEXT.c_str()), window->tw_area.h - WIDGETS_SPACING - TTF_FontHeight(draw_title_ttf->ttf_font));
	draw_title_ttf->draw_string(window->win, VERSION_TEXT.c_str(), versionStringPos);
	runOnce(onReady);
}

void onWindowResize(int dw, int dh)
{
//	cout << "resize event" << endl;
	planetarium->widen(dw, dh);
	txtBodies->widen(0, dh);

	toolbarNorthLayout->pack();
	toolbarSouthLayout->position.y = window->tw_area.h - (1.25*TOOLBAR_SIZE - 2*WIDGETS_SPACING);
	toolbarSouthLayout->pack();

	window->draw_blit_recur();
}

void onKeyEvent(SDL_keysym *key, bool down)
{
	switch (key->sym) {
		//translation
		case SDLK_UP:
			planetarium->currentViewportTranlationRate.y = down? - (double) planetarium->viewportTranlationRateValue : 0;
			break;
		case SDLK_DOWN:
			planetarium->currentViewportTranlationRate.y = down?   (double) planetarium->viewportTranlationRateValue : 0;
			break;
		case SDLK_LEFT:
			planetarium->currentViewportTranlationRate.x = down? - (double) planetarium->viewportTranlationRateValue : 0;
			break;
		case SDLK_RIGHT:
			planetarium->currentViewportTranlationRate.x = down?   (double) planetarium->viewportTranlationRateValue : 0;
			break;
		//zoom
		case SDLK_PLUS:
		case SDLK_KP_PLUS:
		case SDLK_PAGEUP:
			planetarium->currentViewportZoomChangeRate = down? 1.0 + planetarium->viewportZoomChangeRateValue : 1.0;
			break;
		case SDLK_MINUS:
		case SDLK_KP_MINUS:
		case SDLK_PAGEDOWN:
			planetarium->currentViewportZoomChangeRate = down? 1.0 - planetarium->viewportZoomChangeRateValue : 1.0;
			break;
		//key bindings
		case SDLK_a:
			if(down) onButtonPressed(btnAddBody);
			break;
		case SDLK_r:
			if(down) onButtonPressed(btnAddRandom);
			break;
		case SDLK_o:
			if(down) onButtonPressed(btnRecolorAll);
			break;
		case SDLK_p:
			if(down) onButtonPressed(planetarium->running? btnPause: btnRun);
			break;
		case SDLK_t:
			if(down) onCheckBoxPressed(chckTraceOrbit, true);
			break;
		case SDLK_d:
			if(down)
			{
				planetarium->orbitTracer.traceLength *= 2;
				spnTraceLength->refresh();
			}
			break;
		case SDLK_h:
			if(down)
			{
				planetarium->orbitTracer.traceLength *= 0.5;
				spnTraceLength->refresh();
			}
			break;
		default:break;
	}
}

void onButtonPressed(Button* btn)
{
	if(btn == btnAddBody)
	{
		planetarium->running = false;
		planetarium->bodyCreationState = Planetarium::POSITION_SELECTION;
	}

	if(btn == btnAddRandom)
	{
		double az = 1/planetarium->viewportZoom;
		double diameter = (planetarium->bodyCreationDiameterRatio * az) * Planetarium::BODY_CREATION_DIAMETER_FACTOR;
		double mass = (Math::PI/6.0) * planetarium->bodyCreationDensity * diameter * diameter * diameter;
		double speed = 10; //this should be parametrized
		Vector2D randomPosition(randomBetween(0, planetarium->tw_area.w), randomBetween(0, planetarium->tw_area.h));
		Vector2D randomVelocity(randomBetween(-speed * az, speed * az), randomBetween(-speed * az, speed * az));
		randomPosition.scale(az).add(planetarium->viewportPosition);

		planetarium->addCustomBody(new Body2D(mass, diameter, randomPosition, randomVelocity, Vector2D::NULL_VECTOR), SDL_util::getRandomColor());
	}

	if(btn == btnRecolorAll)
	{
		planetarium->recolorAllBodies();
	}

	if(btn == btnRun)
	{
		planetarium->setRunning();
	}

	if(btn == btnPause)
	{
		planetarium->setRunning(false);
	}
}

void onCheckBoxPressed(CheckBox* chck) { onCheckBoxPressed(chck, false); } //callback matching the function pointer

void onCheckBoxPressed(CheckBox* chck, bool fake)
{
	if(fake) *(chck->d) = not *(chck->d); //switch the boolean if the call is a fake-press

	if(chck == chckTraceOrbit)
	{
		//enable tracing parameters editing
		chckTraceOrbit->draw_blit_upd();
	}
}

void onDropDownMenuButton(RButWin* btn, int nr, int fire)
{
	if(btn == ddmTraceStyle->cmdMenu->buttons && fire)
	{
		RButton* rbtn = btn->act_button();
		if(string(rbtn->label.str) == "Linear")
		{
			planetarium->orbitTracer.style = Planetarium::OrbitTracer::LINEAR;
			ddmTraceStyle->cmdMenu->src->label = "Linear";
			ddmTraceStyle->cmdMenu->src->draw_blit_upd();
		}
		if(string(rbtn->label.str) == "Point")
		{
			planetarium->orbitTracer.style = Planetarium::OrbitTracer::POINT;
			ddmTraceStyle->cmdMenu->src->label = "Point";
			ddmTraceStyle->cmdMenu->src->draw_blit_upd();
		}
	}
}

void onTabPressed(RExtButton*,bool is_act)
{

}

void onUserEvent(int cmd,int param,int param2)
{
	if(cmd == CPlanetsGUI::USER_EVENT_ID__REDRAW_COMPONENT)
	{
		if(param == PLANETARIUM_ID)
		{
			planetarium->doRefresh();
		}
	}
}

void onPlanetariumBodyCollision(vector<Body2D>& collidingList, Body2D& resultingMerger)
{
	txtBodies->reset();
	vector<Body2D> bodies = planetarium->getBodies();
	foreach(Body2D&, body, vector<Body2D>, bodies)
	{
		txtBodies->add_text(body.toString().c_str(), false);
	}
	txtBodies->draw_blit_upd(); //xxx call to draw_blit_upd() here may cause race conditions
}

void onPlanetariumBodyCreation(Body2D& createdBody)
{
	txtBodies->add_text(createdBody.toString().c_str(), true);
}

void onReady()
{
	spnTraceLength->refresh();
	spnBodyDiameter->refresh();
	spnBodyDensity->refresh();
}
