/*
 * main_window.cpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#include "main_window.hpp"

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <climits>

#include "futil/futil.hpp"
#include "SDL_widgets/SDL_widgets.h"
#include "SDL_util.hpp"

#include "planetarium.hpp"
#include "program_io.hpp"
#include "widgets/widgets_util.hpp"
#include "widgets/flow_layout.hpp"
#include "widgets/spinner.hpp"
#include "widgets/drop_menu.hpp"
#include "widgets/tab_set.hpp"
#include "widgets/label_win.hpp"
#include "widgets/toogle_button.hpp"
#include "widgets/file_dialog.hpp"
#include "widgets/scrollable_pane.hpp"

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

using std::cout;
using std::endl;
using std::vector;

using Math::randomBetween;

using WidgetsExtra::packLabeledComponent;
using WidgetsExtra::setComponentPosition;
using WidgetsExtra::LabeledComponentPacker;
using WidgetsExtra::Layout;
using WidgetsExtra::FlowLayout;
using WidgetsExtra::Spinner;
using WidgetsExtra::DropDownMenu;
using WidgetsExtra::DropDownMenuFactory;
using WidgetsExtra::TabSet;
using WidgetsExtra::LabelWin;
using WidgetsExtra::ToogleButton;
using WidgetsExtra::FileDialog;
using WidgetsExtra::ScrollablePane;
using WidgetsExtra::DialogBgrWin;

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
const int USER_EVENT_ID__UPDATE_BODIES_LIST = 160;
string VERSION_TEXT, FULL_ABOUT_TEXT; //not really a constant, but still

//  ================ VARIABLES ===============
Rect genericButtonSize(0, 0, TOOLBAR_SIZE, TOOLBAR_SIZE);

//  ================ COMPONENTS ================
TopWin* window; // The program window

FlowLayout* toolbarNorthLayout;
Button* btnNew, *btnLoad, *btnSave, *btnRun, *btnPause, *btnAbout;
FileDialog* dialogLoad, *dialogSave;

TabSet* tabs;

BgrWin* tabBodies;
TextWin* txtBodies;
ScrollablePane* sclpBodies;

BgrWin* tabOptions;
CheckBox* chckTraceOrbit;
Spinner<unsigned>* spnTraceLength;
Spinner<double>* spnBodyDiameter, *spnBodyDensity;
Spinner<double>* spnTimeStep, *spnGravity;
Spinner<long>* spnDisplayPeriod;
DropDownMenu* ddmTraceStyle;

Planetarium* planetarium;

FlowLayout* toolbarRight;
Button* btnAddBody, *btnAddRandom, *btnRecolorAll;

FlowLayout* toolbarSouthLayout;
ToogleButton* tgbTraceOrbit;
Button* btnDoubleTraceLength, *btnHalveTraceLentgh;
ToogleButton* tgbAA;


DialogBgrWin* dialogAbout;
ScrollablePane* sclpAboutLicense;
Button* btnAboutOk;

//  ================ FUNCTION PROTOTYPES ================
void draw(); // The drawing function.
void drawAboutDialog(BgrWin* dialog);
void onWindowResize(int dw, int dh); // callback for window resizing events
void onKeyEvent(SDL_keysym *key,bool down);
void onButtonPressed(Button* btn);
void onCheckBoxPressed(CheckBox* chck);
void onCheckBoxPressed(CheckBox* chck, bool fake);
void onDropDownMenuButton(RButWin*,int nr,int fire);
void onUserEvent(int cmd,int param,int param2);
void onPlanetariumBodyCollision(vector<Body2D>& collidingList, Body2D& resultingMerger);
void onPlanetariumBodyCreation(Body2D& createdBody);
void onReady();

void onFileChosenOpenUniverse(FileDialog* dialog);
void onFileChosenSaveUniverse(FileDialog* dialog);

void refreshAllTxtBodies();
void updateSizeTxtBodies();
void closeDialogBgrWin(Button* btn);
void replaceUniverse(Universe2D* universe);

//  ==================== PLANETARIUM LISTENER ===========================

struct CustomUniverseListener extends Planetarium::UniverseEventListener
{
	void onBodyCollision(vector<Body2D>& collidingList, Body2D& resultingMerger) { onPlanetariumBodyCollision(collidingList, resultingMerger); }
	void onBodyCreation(Body2D& createdBody) { onPlanetariumBodyCreation(createdBody); }
};

// ================ CPlanetsGUI::MainWindow namespace ================
void CPlanets::showMainWindow()
{
	Rect windowSize(0, 0, 640, 480);
	window = new TopWin("cplanets", windowSize, SDL_INIT_VIDEO, SDL_RESIZABLE, draw, null, SDLMAIN_STREAM_WORKAROUND);
	handle_rsev = onWindowResize; //set callback for window resize
	handle_kev = onKeyEvent; //set callback for keyboard events
	handle_uev = onUserEvent;
	VERSION_TEXT ="v"+CPLANETS_VERSION;

	LabeledComponentPacker packer(TOOLBAR_SIZE-2*WIDGETS_SPACING);

	//+++++++++++++++ North toolbar
	toolbarNorthLayout = new FlowLayout(WIDGETS_SPACING, WIDGETS_SPACING);

	btnNew = new Button(window, 0, genericButtonSize, "New", onButtonPressed);
	packer.pack(btnNew);
	toolbarNorthLayout->addComponent(btnNew);

	btnLoad = new Button(window, 0, genericButtonSize, "Load", onButtonPressed);
	packer.pack(btnLoad);
	toolbarNorthLayout->addComponent(btnLoad);

	btnSave = new Button(window, 0, genericButtonSize, "Save", onButtonPressed);
	packer.pack(btnSave);
	toolbarNorthLayout->addComponent(btnSave);

	toolbarNorthLayout->addComponent(new Layout::Spacer(toolbarNorthLayout));

	btnRun = new Button(window, 0, genericButtonSize, "Run", onButtonPressed);
	packer.pack(btnRun);
	toolbarNorthLayout->addComponent(btnRun);

	btnPause = new Button(window, 0, genericButtonSize, "Pause", onButtonPressed);
	packer.pack(btnPause);
	toolbarNorthLayout->addComponent(btnPause);

	btnAbout = new Button(window, 0, genericButtonSize, "About", onButtonPressed);
	packer.pack(btnAbout);
	toolbarNorthLayout->addComponent(btnAbout);

	toolbarNorthLayout->pack();


	// ****Planetarium****
	Rect planetariumSize(
		BODIES_PANEL_WIDTH + WIDGETS_SPACING,
		TOOLBAR_SIZE + 0.5*WIDGETS_SPACING,
		windowSize.w - (BODIES_PANEL_WIDTH + TOOLBAR_SIZE + 2*WIDGETS_SPACING),
		windowSize.h - (2.1*TOOLBAR_SIZE)
	);
	planetarium = new Planetarium(window, planetariumSize, PLANETARIUM_ID);
	planetarium->addUniverseEventListener(new CustomUniverseListener());


	//+++++++++++++++ Tabs
	tabs = new TabSet(window, WIDGETS_SPACING, TOOLBAR_SIZE + 0.5*WIDGETS_SPACING, 0, 22);
	Rect sizeTab(
		tabs->layout.position.x,
		tabs->layout.position.y + tabs->layout.maxHeight,
		BODIES_PANEL_WIDTH - WIDGETS_SPACING,
		planetariumSize.h - tabs->layout.maxHeight
	); //todo Rearrange tabs position, (8) and (TOOLBAR_SIZE - 8) should be removed

	// Tab bodies
	tabBodies = new BgrWin(window, sizeTab, null, TabSet::drawTabStyleBgrWin, null, null, null, window->bgcol);
	tabs->addTab("Bodies", tabBodies);

	sclpBodies = new ScrollablePane(tabBodies, Style(0, 3*WIDGETS_SPACING), Rect(2, 2, sizeTab.w - 3, sizeTab.h - 3), window->bgcol);
	sclpBodies->setScrollbarHorizontalVisible(false);

	Rect txtBodiesSize(
			0,
			0,
			sclpBodies->tw_area.w,
			sclpBodies->tw_area.h);
	txtBodies = new TextWin(&sclpBodies->content, 0, txtBodiesSize, SHRT_MAX, null);

	// Tab options
	tabOptions = new BgrWin(window, sizeTab, null, TabSet::drawTabStyleBgrWin, null, null, null, window->bgcol);
	tabs->addTab("Options", tabOptions);

	tabs->layout.pack();
	tabs->setActiveTab(tabBodies);

	LabelWin lblOrbitTracing(tabOptions, Rect(), "Orbit tracing");
	lblOrbitTracing.setTextRenderer(draw_title_ttf);
	setComponentPosition(&lblOrbitTracing, WIDGETS_SPACING, WIDGETS_SPACING);

	chckTraceOrbit = new CheckBox(tabOptions, 0, genericButtonSize, "Show orbit trace", onCheckBoxPressed);
	chckTraceOrbit->d = &(planetarium->orbitTracer.isActive);  // binds the checkbox to the variable
	setComponentPosition(chckTraceOrbit, lblOrbitTracing.area.x, lblOrbitTracing.area.y + lblOrbitTracing.tw_area.h + WIDGETS_SPACING);
	packLabeledComponent(chckTraceOrbit);

	spnTraceLength = new Spinner<unsigned>(tabOptions, Rect(0, 0, 3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Trace length:");
	spnTraceLength->setPosition(Point(chckTraceOrbit->area.x, chckTraceOrbit->area.y + chckTraceOrbit->tw_area.h + WIDGETS_SPACING));
	spnTraceLength->setValue(&(planetarium->orbitTracer.traceLength));

	DropDownMenuFactory factory;
	factory.setLabel("Trace style: ", true);
	factory.setAppearance(DropDownMenuFactory::COMBOBOX);
	factory.setSize(Rect(40, 40, 100, 20));
	factory.addItem("Linear");
	factory.addItem("Point");
	factory.setCallback(onDropDownMenuButton);
	ddmTraceStyle = factory.createAt(tabOptions);
	ddmTraceStyle->setPosition(Point(spnTraceLength->getPosition().x + spnTraceLength->getSize().w + WIDGETS_SPACING, spnTraceLength->getPosition().y));
	ddmTraceStyle->offset.y = -10;

	LabelWin lblBodyCreation(tabOptions, Rect(), "Body creation parameters");
	lblBodyCreation.setTextRenderer(draw_title_ttf);
	setComponentPosition(&lblBodyCreation, spnTraceLength->getPosition().x, ddmTraceStyle->getPosition().y + ddmTraceStyle->getSize().h + 2*WIDGETS_SPACING);

	spnBodyDiameter = new Spinner<double>(tabOptions, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Diameter:");
	spnBodyDiameter->setPosition(Point(lblBodyCreation.area.x, lblBodyCreation.area.y + lblBodyCreation.tw_area.h + WIDGETS_SPACING));
	spnBodyDiameter->setValue(&(planetarium->bodyCreationDiameterRatio));
	spnBodyDiameter->setStepValue(0.1);
	spnBodyDiameter->offset.y -= 2;

	spnBodyDensity = new Spinner<double>(tabOptions, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Density:");
	spnBodyDensity->setPosition(Point(spnBodyDiameter->getPosition().x + spnBodyDiameter->getSize().w + WIDGETS_SPACING, spnBodyDiameter->getPosition().y));
	spnBodyDensity->setValue(&(planetarium->bodyCreationDensity));
	spnBodyDensity->setStepValue(0.1);
	spnBodyDensity->offset.y -= 2;

	LabelWin lblSimulationParameters(tabOptions, Rect(), "Simulation parameters");
	lblSimulationParameters.setTextRenderer(draw_title_ttf);
	setComponentPosition(&lblSimulationParameters, spnBodyDiameter->getPosition().x, spnBodyDiameter->getPosition().y + spnBodyDiameter->getSize().h + 3*WIDGETS_SPACING);

	spnTimeStep = new Spinner<double>(tabOptions, Rect(0,0,2.4*TOOLBAR_SIZE, TOOLBAR_SIZE), "Time step:");
	spnTimeStep->setPosition(Point(lblSimulationParameters.area.x, lblSimulationParameters.area.y + lblSimulationParameters.tw_area.h + WIDGETS_SPACING));
	spnTimeStep->setValue(&(planetarium->physics->physics2DSolver->timestep));
	spnTimeStep->setStepValue(0.1);
	spnTimeStep->offset.y -= 2;

	spnGravity = new Spinner<double>(tabOptions, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Gravity:");
	spnGravity->setPosition(Point(spnTimeStep->getPosition().x + spnTimeStep->getSize().w + WIDGETS_SPACING, spnTimeStep->getPosition().y));
	spnGravity->setValue(&(planetarium->physics->universe.gravity));
	spnGravity->setStepValue(0.1);
	spnGravity->offset.y -= 2;

	spnDisplayPeriod = new Spinner<long>(tabOptions, Rect(0,0,3.2*TOOLBAR_SIZE, TOOLBAR_SIZE), "Display period:");
	spnDisplayPeriod->setPosition(Point(spnTimeStep->getPosition().x, spnTimeStep->getPosition().y + spnTimeStep->getSize().h + WIDGETS_SPACING));
	spnDisplayPeriod->setValue(&(planetarium->sleepingTime));
	spnDisplayPeriod->offset.y -= 2;

	//+++++++++++++++ East (right) toolbar
	toolbarRight = new FlowLayout(planetariumSize.x + planetariumSize.w + WIDGETS_SPACING, TOOLBAR_SIZE + 0.5*WIDGETS_SPACING);
	toolbarRight->orientation = Layout::VERTICAL;

	Rect sideButtonSize(0, 0, TOOLBAR_SIZE - WIDGETS_SPACING, TOOLBAR_SIZE - 2*WIDGETS_SPACING);

	toolbarRight->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::VERTICAL, TOOLBAR_SIZE)));

	btnAddBody = new Button(window, 0, sideButtonSize, "Add", onButtonPressed);
	toolbarRight->addComponent(btnAddBody);

	btnAddRandom = new Button(window, 0, sideButtonSize, "AdR", onButtonPressed);
	toolbarRight->addComponent(btnAddRandom);

	toolbarRight->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::VERTICAL, TOOLBAR_SIZE)));

	btnRecolorAll = new Button(window, 0, sideButtonSize, "Rcl", onButtonPressed);
	toolbarRight->addComponent(btnRecolorAll);

	toolbarRight->pack();

	//+++++++++++++++ South toolbar
	toolbarSouthLayout = new FlowLayout(WIDGETS_SPACING, windowSize.h - (1.25*TOOLBAR_SIZE - 2*WIDGETS_SPACING));
	toolbarSouthLayout->alignment = FlowLayout::MIDDLE;

	tgbTraceOrbit = new ToogleButton(window, 0, genericButtonSize, "Trace orbit", onCheckBoxPressed);
	tgbTraceOrbit->d = &(planetarium->orbitTracer.isActive);  // binds the checkbox to the variable
	packer.pack(tgbTraceOrbit, tgbTraceOrbit->label);
	toolbarSouthLayout->addComponent(tgbTraceOrbit);

	btnDoubleTraceLength = new Button(window, 0, genericButtonSize, "Db.T", onButtonPressed);
	packer.pack(btnDoubleTraceLength);
	toolbarSouthLayout->addComponent(btnDoubleTraceLength);

	btnHalveTraceLentgh = new Button(window, 0, genericButtonSize, "Hv.T", onButtonPressed);
	packer.pack(btnHalveTraceLentgh);
	toolbarSouthLayout->addComponent(btnHalveTraceLentgh);

	toolbarSouthLayout->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::HORIZONTAL, TOOLBAR_SIZE)));

	tgbAA = new ToogleButton(window, 0, genericButtonSize, "Smooth", onCheckBoxPressed);
	tgbAA->d = &(planetarium->tryAA);
	packer.pack(tgbAA, tgbAA->label);
	toolbarSouthLayout->addComponent(tgbAA);

	toolbarSouthLayout->pack();

	vector<string> strFiletypes;
	strFiletypes.push_back("Plain text (*.txt)");
	dialogLoad = new FileDialog(FileDialog::SELECT_FILE, onFileChosenOpenUniverse, strFiletypes);
	dialogSave = new FileDialog(FileDialog::SAVE_FILE, onFileChosenSaveUniverse, strFiletypes);

	FULL_ABOUT_TEXT = "Version "+CPLANETS_VERSION+"\n\nThis program is inspired by Yaron Minsky's \"planets\" program.\n\n" + CPLANETS_LICENSE;
	dialogAbout = new DialogBgrWin(Rect(0,0,400,300), "About cplanets");

	btnAboutOk = new Button(dialogAbout, 0, genericButtonSize, "Close", closeDialogBgrWin);
	packLabeledComponent(btnAboutOk);
	setComponentPosition(btnAboutOk, 0.5*(400-btnAboutOk->tw_area.w), 300-btnAboutOk->tw_area.h-WIDGETS_SPACING);

	Rect rectSclpAboutLicense(
			WIDGETS_SPACING,
			dialogAbout->titleBarArea.h + WIDGETS_SPACING,
			400-2*WIDGETS_SPACING,
			300-3*WIDGETS_SPACING-dialogAbout->titleBarArea.h-btnAboutOk->tw_area.h);

	sclpAboutLicense = new ScrollablePane(dialogAbout, 0, rectSclpAboutLicense, window->bgcol);
	sclpAboutLicense->content.display_cmd = drawAboutDialog;
	sclpAboutLicense->setScrollbarHorizontalVisible(false);

//	print_h(); //DEBUG

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

void closeDialogBgrWin(Button* btn)
{
	DialogBgrWin* self = static_cast<DialogBgrWin*>(btn->parent);
	self->setVisible(false);
	if(self->onClosedCallback != null)
		self->onClosedCallback(self);
}

void drawAboutDialog(BgrWin* bw)
{
	BgrWin* dialog = &sclpAboutLicense->content;
	WidgetsExtra::drawDefaultBgrWin(bw);

	draw_title_ttf->draw_string(dialog->win, "cplanets, a interactive program to play with gravitation", Point(WIDGETS_SPACING, WIDGETS_SPACING));

	unsigned lf = 0, cr = 0, ln = 2;

	while( lf + cr < FULL_ABOUT_TEXT.size() )
	{
		bool exceed = false;

		if(FULL_ABOUT_TEXT.c_str()[lf+cr] == '\n' ||  lf + cr == FULL_ABOUT_TEXT.size()-1) // line feed or last draw
			exceed = true;
		else
		{
			string substr = String::replaceAll(FULL_ABOUT_TEXT.substr(lf, cr+1), "\n", " ");
			if((unsigned) draw_ttf->text_width(substr.c_str()) > (dialog->tw_area.w - 2*WIDGETS_SPACING))
				exceed = true;
		}

		if(exceed)
		{
			//draw from text pointer, 'cr' caracters
			string substr = String::replaceAll(FULL_ABOUT_TEXT.substr(lf, cr), "\n", " ");
			draw_ttf->draw_string(dialog->win, substr.c_str(), Point(WIDGETS_SPACING, ln*TTF_FontHeight(draw_ttf->ttf_font)));
			lf += cr; //increase text pointer
			cr = 0; //"caret" position is reset
			ln++; //line number incremented
		}

		cr++; //increase caret position
	}
}

void onWindowResize(int dw, int dh)
{
//	cout << "resize event" << endl;
	if(dialogAbout->parent != null) dialogAbout->hide();

	toolbarNorthLayout->pack();

	planetarium->widen(dw, dh);

	//todo make a widenAll() method on TabSet
	tabBodies->widen(0, dh);
	sclpBodies->widen(0, dh);
	updateSizeTxtBodies();

	tabOptions->widen(0, dh);

	toolbarRight->position.x += dw;
	toolbarRight->pack();

	toolbarSouthLayout->position.y += dh;
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
			if(down) onButtonPressed(btnDoubleTraceLength);
			break;
		case SDLK_h:
			if(down) onButtonPressed(btnHalveTraceLentgh);
			break;
		case SDLK_s:
			if(down) onCheckBoxPressed(tgbAA, true);
			break;
		default:break;
	}
}

void onButtonPressed(Button* btn)
{
	if(btn == btnAbout)
	{
		setComponentPosition(dialogAbout, window->tw_area.w*0.5 - 200, window->tw_area.h*0.5 - 150);
		if(dialogAbout->parent == null)
		{
			dialogAbout->keep_on_top();
			dialogAbout->draw_blit_recur();
			dialogAbout->upd();
		}
		else if(dialogAbout->hidden)
			dialogAbout->show();
		else
			dialogAbout->hide();
	}

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

	if(btn == btnDoubleTraceLength)
	{
		planetarium->orbitTracer.traceLength *= 2;
		spnTraceLength->refresh();
	}
	if(btn == btnHalveTraceLentgh)
	{
		planetarium->orbitTracer.traceLength *= 0.5;
		spnTraceLength->refresh();
	}

	if(btn == btnNew)
	{
		replaceUniverse(new Universe2D());
	}

	//fixme there's got to be a better way to avoid file_chooser behind FileDialog bug
	//xxx workaround for file_chooser behind FileDialog bug
	if(btn == btnLoad || btn == btnSave)
	{
		static bool once = false;
		if(not once)
		{
			dialogSave->setPositionOnCenter();
			dialogSave->setVisible();
			dialogSave->hide();
			dialogLoad->setPositionOnCenter();
			dialogLoad->setVisible();
			dialogLoad->hide();
			once = true;
		}
	}

	if(btn == btnLoad)
	{
		if(not dialogSave->hidden) return;
		dialogLoad->setPositionOnCenter();
		dialogLoad->setVisible();
		onButtonPressed(btnPause);
	}

	if(btn == btnSave)
	{
		if(not dialogLoad->hidden) return;
		dialogSave->setPositionOnCenter();
		dialogSave->setVisible();
		onButtonPressed(btnPause);
	}
}

void onCheckBoxPressed(CheckBox* chck) { onCheckBoxPressed(chck, false); } //callback matching the function pointer

void onCheckBoxPressed(CheckBox* chck, bool fake)
{
	if(fake) //fake means not triggered by the user action on the given CheckBox
	{
		*(chck->d) = not *(chck->d); //switch the boolean if the call is a fake-press
		chck->draw_blit_upd(); //update the widget graphics
	}

	if(chck == chckTraceOrbit || tgbTraceOrbit) //update each other graphics as they share value
	{
		tgbTraceOrbit->draw_blit_upd();
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

void onUserEvent(int cmd,int param,int param2)
{
	if(cmd == Planetarium::USER_EVENT_ID__REDRAW_REQUESTED)
	{
		if(param == planetarium->id.id1) //kind of unnecessary, we currently have only one instance of planetarium
			planetarium->doRefresh();
	}

	if(cmd == ::USER_EVENT_ID__UPDATE_BODIES_LIST)
	{
		updateSizeTxtBodies();
		txtBodies->draw_blit_upd();
	}
}

void onPlanetariumBodyCollision(vector<Body2D>& collidingList, Body2D& resultingMerger)
{
	refreshAllTxtBodies();
}

void onPlanetariumBodyCreation(Body2D& createdBody)
{
	txtBodies->add_text(createdBody.toString().c_str(), false);
	send_uev(::USER_EVENT_ID__UPDATE_BODIES_LIST);
}

void onReady()
{
	spnTraceLength->refresh();
	spnBodyDiameter->refresh();
	spnBodyDensity->refresh();
	spnTimeStep->refresh();
	spnGravity->refresh();
	spnDisplayPeriod->refresh();
}


void onFileChosenOpenUniverse(FileDialog* dialog)
{
	if(not dialog->selectedFilename.empty())
	{
		if(FileInputStream(dialog->selectedFilename.c_str()).good())
		{
			Universe2D* u = ApplicationIO::load(string(dialog->selectedFilename.c_str()), ApplicationIO::FORMAT_DEFAULT);

			if(u != null)
				replaceUniverse(u);

			else alert("Invalid file!");
		}
		else alert("File doesn't exist or isn't readable.");
	}
	SDL_util::setWindowTitle("cplanets"); //todo maybe put filename on title
	onButtonPressed(btnRun);
}

void onFileChosenSaveUniverse(FileDialog* dialog)
{
	if(not dialog->selectedFilename.empty())
	{
		// query 'dialog->selectedType' for selected file type
		ApplicationIO::save(planetarium->physics->universe, dialog->selectedFilename);
	}
	onButtonPressed(btnRun);
}

void refreshAllTxtBodies()
{
	txtBodies->reset();
	vector<Body2D> bodies = planetarium->getBodies();
	foreach(Body2D&, body, vector<Body2D>, bodies)
	{
		txtBodies->add_text(body.toString().c_str(), false);
	}
	send_uev(::USER_EVENT_ID__UPDATE_BODIES_LIST);
}

void updateSizeTxtBodies()
{
	unsigned height2 = sclpBodies->tw_area.h;

	if(txtBodies->linenr >= 0) //if there are texts, make the height of the content to be at least the needed size for the texts
		height2 = Math::max((txtBodies->linenr+1)*TDIST + 4, (int) sclpBodies->tw_area.h);

	if(height2 != txtBodies->tw_area.h) //avoids unneeded widening
	{
		txtBodies->widen(0, height2 - txtBodies->tw_area.h);
		sclpBodies->widenContent(0, height2 - sclpBodies->content.tw_area.h);
	}
}

void replaceUniverse(Universe2D* universe)
{
	onButtonPressed(btnPause);
	planetarium->setUniverse(universe);
	refreshAllTxtBodies();
	spnTimeStep->setValue(&(planetarium->physics->physics2DSolver->timestep));
}
