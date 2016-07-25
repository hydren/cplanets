/*
 * main_window.cpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */

#include "main_window.hpp"

#include "hotfixes.h" //macros to enable/disable hotfixes for SDL-related issues

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <climits>

#include "futil/futil.hpp"
#include "SDL_widgets/SDL_widgets.h"
#include "SDL_util.hpp"

#include "planetarium_pane.hpp"
#include "program_io.hpp"
#include "widgets/widgets_util.hpp"
#include "widgets/widgets_debug.hpp"

#include "widgets/flow_layout.hpp"
#include "widgets/spinner.hpp"
#include "widgets/drop_menu.hpp"
#include "widgets/tab_set.hpp"
#include "widgets/label_win.hpp"
#include "widgets/toogle_button.hpp"
#include "widgets/file_dialog.hpp"
#include "widgets/scrollable_pane.hpp"
#include "widgets/list_win.hpp"
#include "widgets/list_selection_model_extra.hpp"

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
using WidgetsExtra::ListWin;

typedef Planetarium::Body2DClone Body2DClone;

//**********************************************************************************
// workaround to reroute output stream to console
FILE* workaround_sdl_stream_file = null;
void workaround_sdl_stream_file_close() // part of workaround
{
	if(workaround_sdl_stream_file != null) // if file var was used, close the file
		fclose(workaround_sdl_stream_file);
}

//**********************************************************************************

//  ============= FUNCTION PROTOTYPES ================
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

void onFileChosenOpenUniverse(FileDialog* dialog);
void onFileChosenSaveUniverse(FileDialog* dialog);

void refreshAllTxtBodies();
void updateSizeTxtBodies();
void onListSelectionChanged(unsigned, unsigned);
void onBodyReFocusing();
void adjustAboutDialog();
void closeDialogBgrWin(Button* btn);
void replaceUniverse(Universe2D* universe);
void addRandomBody();
int keepAddingRandomBodyWhilePressed(void* unused);

void onSDLInit();

//  =========== PLANETARIUM LISTENER ===========================

struct CustomListener extends Planetarium::UniverseEventListener, WidgetsExtra::ListSelectionModel::Listener
{
	void onBodyCollision(vector<Body2D>& collidingList, Body2D& resultingMerger) { onPlanetariumBodyCollision(collidingList, resultingMerger); }
	void onBodyCreation(Body2D& createdBody) { onPlanetariumBodyCreation(createdBody); }
	void onBodyDeletion(Body2D* deletedBody) { refreshAllTxtBodies(); }
	void onBodyReFocus(){ onBodyReFocusing(); }
	void onChange(unsigned index, unsigned endIndex) { onListSelectionChanged(index, endIndex); }
};

// ================ CONSTANTS ================
const unsigned TOOLBAR_SIZE = 32; // TOOLBAR_SIZE is used as size reference for buttons, spacing, etc
const unsigned WIDGETS_SPACING = 4;
const unsigned BODIES_PANEL_WIDTH = TOOLBAR_SIZE * 7;
const int PLANETARIUM_ID = 959;
const int USER_EVENT_ID__UPDATE_BODIES_LIST = 160;
const double DEFAULT_GRAVITY = 9.807;
string VERSION_TEXT, FULL_ABOUT_TEXT; //not really a constant, but still

SDL_Surface* APP_LOGO;

//  ================ VARIABLES ===============
Rect genericButtonSize(0, 0, TOOLBAR_SIZE, TOOLBAR_SIZE); //useful to reuse
CustomListener customListener;
bool aux_isPressed_SDLK_r = false;

//  ================ COMPONENTS ================
TopWin* window; // The program window

FlowLayout* toolbarNorthLayout;
Button* btnNew, *btnLoad, *btnSave, *btnRun, *btnPause, *btnAbout;
FileDialog* dialogLoad, *dialogSave;

TabSet* tabs;

BgrWin* tabBodies;
ListWin<Body2DClone>* txtBodies;
ScrollablePane* sclpBodies;

BgrWin* tabOptions;
CheckBox* chckTraceOrbit;
Spinner<unsigned>* spnTraceLength;
DropDownMenu* ddmTraceStyle;
Spinner<double>* spnBodyDiameter, *spnBodyDensity, *spnBodyVelocity;
Spinner<double>* spnTimeStep, *spnGravity;
Spinner<long>* spnStepDelay;
Spinner<short>* spnFPS;
DropDownMenu* ddmIntegrationMethod;
CheckBox* chckLegacyParameters;
Spinner<long>* spnDisplayPeriod, *spnIterPerDisplay;

PlanetariumPane* planetariumPane;
Planetarium* planetarium; ///helper pointer

FlowLayout* toolbarRight;
Button* btnAddBody, *btnAddRandom, *btnRemove, *btnRecolorAll, *btnFollowSelection, *btnResetReferenceFrame;

FlowLayout* toolbarSouthLayout;
ToogleButton* tgbTraceOrbit;
Button* btnDoubleTraceLength, *btnHalveTraceLentgh;
ToogleButton* tgbAA;


DialogBgrWin* dialogAbout;
ScrollablePane* sclpAboutLicense;
Button* btnAboutOk;
vector<string>* dialogAboutTextLines = null;


// ================ CPlanetsGUI::MainWindow namespace ================
void CPlanets::showMainWindow()
{
	Rect windowSize(0, 0, 640, 480);
	window = new TopWin("cplanets", windowSize, SDL_INIT_VIDEO, SDL_RESIZABLE, draw, null, onSDLInit);
	handle_rsev = onWindowResize; //set callback for window resize
	handle_kev = onKeyEvent; //set callback for keyboard events
	handle_uev = onUserEvent;
	VERSION_TEXT ="v"+CPLANETS_VERSION;
	APP_LOGO = SDL_util::loadBitmap("data/icon.bmp", &SDL_util::Color::LIME);

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

	btnAbout = new Button(window, 0, genericButtonSize, "About", onButtonPressed);
	packer.pack(btnAbout);
	toolbarNorthLayout->addComponent(btnAbout);

	toolbarNorthLayout->addComponent(new Layout::Spacer(toolbarNorthLayout));

	btnRun = new Button(window, 0, genericButtonSize, "Run", onButtonPressed);
	packer.pack(btnRun);
	toolbarNorthLayout->addComponent(btnRun);

	btnPause = new Button(window, 0, genericButtonSize, "Pause", onButtonPressed);
	packer.pack(btnPause);
	toolbarNorthLayout->addComponent(btnPause);

	toolbarNorthLayout->pack();


	// ****Planetarium****
	Rect planetariumSize(
		BODIES_PANEL_WIDTH + WIDGETS_SPACING,
		TOOLBAR_SIZE + 0.5*WIDGETS_SPACING,
		windowSize.w - (BODIES_PANEL_WIDTH + TOOLBAR_SIZE + 2*WIDGETS_SPACING),
		windowSize.h - (2.1*TOOLBAR_SIZE)
	);
	planetariumPane = new PlanetariumPane(window, planetariumSize, PLANETARIUM_ID);
	planetarium = planetariumPane->planetarium;
	planetarium->listeners.addListener(&customListener);
	planetarium->physics->universe.gravity = DEFAULT_GRAVITY;


	//+++++++++++++++ Tabs
	tabs = new TabSet(window, WIDGETS_SPACING, TOOLBAR_SIZE + 0.5*WIDGETS_SPACING, 0, 22);
	Rect sizeTab(
		tabs->layout.position.x,
		tabs->layout.position.y + tabs->layout.maxHeight,
		BODIES_PANEL_WIDTH - WIDGETS_SPACING,
		planetariumSize.h - tabs->layout.maxHeight
	);

	// Tab bodies
	tabBodies = new BgrWin(window, sizeTab, null, TabSet::drawTabStyleBgrWin, null, null, null, window->bgcol);
	tabs->addTab("Bodies", tabBodies);

	sclpBodies = new ScrollablePane(tabBodies, Style(0, 3*WIDGETS_SPACING), Rect(2, 2, sizeTab.w - 3, sizeTab.h - 3), window->bgcol);
	sclpBodies->setScrollbarHorizontalVisible(false);

	Rect txtBodiesSize(0, 0, sclpBodies->tw_area.w, sclpBodies->tw_area.h);
	txtBodies = new ListWin<Body2DClone>(&sclpBodies->content, 0, txtBodiesSize, String::Callbacks::stringfy_by_method<Body2DClone, &Body2DClone::toString>);
	txtBodies->selectionAdjustmentFunction = WidgetsExtra::GenericSelectionAdjustment::function<Body2DClone>;
	txtBodies->selection.listenerAdd(&customListener);
	txtBodies->selection.onChange = onListSelectionChanged;
	txtBodies->preventRedrawOnClick = true;

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
	setComponentPosition(spnTraceLength, chckTraceOrbit->area.x, chckTraceOrbit->area.y + chckTraceOrbit->tw_area.h + WIDGETS_SPACING + 3);
	spnTraceLength->setValue(&(planetarium->orbitTracer.traceLength), true);

	DropDownMenuFactory factory;
	factory.setLabel("Trace style: ", true);
	factory.setAppearance(DropDownMenuFactory::COMBOBOX);
	factory.setSize(Rect(40, 40, 100, 20));
	factory.addItem("Linear");
	factory.addItem("Dotted");
	factory.setCallback(onDropDownMenuButton);
	ddmTraceStyle = factory.createAt(tabOptions);
	ddmTraceStyle->setPosition(Point(spnTraceLength->area.x + spnTraceLength->tw_area.w + WIDGETS_SPACING, spnTraceLength->area.y - 3));
	ddmTraceStyle->offset.y = -10;

	LabelWin lblBodyCreation(tabOptions, Rect(), "Body creation parameters");
	lblBodyCreation.setTextRenderer(draw_title_ttf);
	setComponentPosition(&lblBodyCreation, spnTraceLength->area.x, ddmTraceStyle->getPosition().y + ddmTraceStyle->getSize().h + 2*WIDGETS_SPACING);

	spnBodyDiameter = new Spinner<double>(tabOptions, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Diameter:");
	setComponentPosition(spnBodyDiameter, lblBodyCreation.area.x, lblBodyCreation.area.y + lblBodyCreation.tw_area.h + WIDGETS_SPACING);
	spnBodyDiameter->setValue(&(planetarium->bodyCreationDiameterRatio), true);
	spnBodyDiameter->setStepValue(0.1);

	spnBodyDensity = new Spinner<double>(tabOptions, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Density:");
	setComponentPosition(spnBodyDensity, spnBodyDiameter->area.x + spnBodyDiameter->tw_area.w + WIDGETS_SPACING, spnBodyDiameter->area.y);
	spnBodyDensity->setValue(&(planetarium->bodyCreationDensity), true);
	spnBodyDensity->setStepValue(0.1);

	spnBodyVelocity = new Spinner<double>(tabOptions, Rect(0,0,5.75*TOOLBAR_SIZE, TOOLBAR_SIZE), "Velocity (for random objects):");
	setComponentPosition(spnBodyVelocity, spnBodyDiameter->area.x, spnBodyDiameter->area.y + spnBodyDiameter->tw_area.h + WIDGETS_SPACING);
	spnBodyVelocity->setValue(10);
	spnBodyVelocity->setStepValue(0.1);

	LabelWin lblSimulationParameters(tabOptions, Rect(), "Simulation parameters");
	lblSimulationParameters.setTextRenderer(draw_title_ttf);
	setComponentPosition(&lblSimulationParameters, spnBodyVelocity->area.x, spnBodyVelocity->area.y + spnBodyVelocity->tw_area.h + 3*WIDGETS_SPACING);

	spnTimeStep = new Spinner<double>(tabOptions, Rect(0,0,2.4*TOOLBAR_SIZE, TOOLBAR_SIZE), "Time step:");
	setComponentPosition(spnTimeStep, lblSimulationParameters.area.x, lblSimulationParameters.area.y + lblSimulationParameters.tw_area.h + WIDGETS_SPACING);
	spnTimeStep->setValue(&(planetarium->physics->physics2DSolver->timestep), true);
	spnTimeStep->setStepValue(0.1);

	spnGravity = new Spinner<double>(tabOptions, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Gravity:");
	setComponentPosition(spnGravity, spnTimeStep->area.x + spnTimeStep->tw_area.w + WIDGETS_SPACING, spnTimeStep->area.y);
	spnGravity->setValue(&(planetarium->physics->universe.gravity), true);
	spnGravity->setStepValue(0.1);

	factory.setLabel("Integration method: ", true);
	factory.setSize(Rect(40, 40, 200, 20));
	factory.removeAllItems();
	typedef AbstractPhysics2DSolver::GenericFactory SolverFactory;
	const_foreach(const SolverFactory*, solverFactory, vector<const SolverFactory*>, AbstractPhysics2DSolver::registeredFactories)
	{
		factory.addItem(solverFactory->solverDisplayName.c_str());
	}
	ddmIntegrationMethod = factory.createAt(tabOptions);
	ddmIntegrationMethod->cmdMenu->src->label = Label(planetarium->physics->physics2DSolver->factory->solverDisplayName.c_str());
	ddmIntegrationMethod->setPosition(Point(spnTimeStep->area.x, spnTimeStep->area.y + spnTimeStep->tw_area.h + WIDGETS_SPACING));
	ddmIntegrationMethod->offset.y = -10;

	spnStepDelay = new Spinner<long>(tabOptions, Rect(0,0,3.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Step delay (ms):");
	setComponentPosition(spnStepDelay, ddmIntegrationMethod->getPosition().x, ddmIntegrationMethod->getPosition().y + ddmIntegrationMethod->getSize().h + WIDGETS_SPACING);
	spnStepDelay->setValue(&(planetarium->stepDelay), true);

	spnFPS = new Spinner<short>(tabOptions, Rect(0,0,2*TOOLBAR_SIZE, TOOLBAR_SIZE), "FPS:");
	setComponentPosition(spnFPS, spnStepDelay->area.x + spnStepDelay->tw_area.w + WIDGETS_SPACING, spnStepDelay->area.y);
	spnFPS->setValue(&(planetarium->fps), true);

	chckLegacyParameters = new CheckBox(tabOptions, 0, genericButtonSize, "Use legacy parameters:", onCheckBoxPressed);
	chckLegacyParameters->d = &(planetarium->legacyControl);  // binds the checkbox to the variable
	setComponentPosition(chckLegacyParameters, spnStepDelay->area.x, spnStepDelay->area.y + spnStepDelay->tw_area.h + WIDGETS_SPACING);
	packLabeledComponent(chckLegacyParameters);

	spnDisplayPeriod = new Spinner<long>(tabOptions, Rect(0,0,3.6*TOOLBAR_SIZE, TOOLBAR_SIZE), "Disp. period (ms):");
	setComponentPosition(spnDisplayPeriod, chckLegacyParameters->area.x, chckLegacyParameters->area.y + chckLegacyParameters->tw_area.h + WIDGETS_SPACING);
	spnDisplayPeriod->setValue(&(planetarium->displayPeriod));

	spnIterPerDisplay = new Spinner<long>(tabOptions, Rect(0,0,2.8*TOOLBAR_SIZE, TOOLBAR_SIZE), "Iter/display:");
	setComponentPosition(spnIterPerDisplay, spnDisplayPeriod->area.x + spnDisplayPeriod->tw_area.w + WIDGETS_SPACING, spnDisplayPeriod->area.y);
	spnIterPerDisplay->setValue(&(planetarium->iterationsPerDisplay));

	//+++++++++++++++ East (right) toolbar
	toolbarRight = new FlowLayout(planetariumSize.x + planetariumSize.w + WIDGETS_SPACING, TOOLBAR_SIZE + 0.5*WIDGETS_SPACING);
	toolbarRight->orientation = Layout::VERTICAL;

	Rect sideButtonSize(0, 0, TOOLBAR_SIZE - WIDGETS_SPACING, TOOLBAR_SIZE - 2*WIDGETS_SPACING);

	toolbarRight->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::VERTICAL, TOOLBAR_SIZE)));

	btnAddBody = new Button(window, 0, sideButtonSize, "Add", onButtonPressed);
	toolbarRight->addComponent(btnAddBody);

	btnAddRandom = new Button(window, 0, sideButtonSize, "AdR", onButtonPressed);
	toolbarRight->addComponent(btnAddRandom);

	btnRemove = new Button(window, 0, sideButtonSize, "Rem", onButtonPressed);
	toolbarRight->addComponent(btnRemove);

	toolbarRight->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::VERTICAL, TOOLBAR_SIZE)));

	btnRecolorAll = new Button(window, 0, sideButtonSize, "Rcl", onButtonPressed);
	toolbarRight->addComponent(btnRecolorAll);

	toolbarRight->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::VERTICAL, TOOLBAR_SIZE)));

	btnFollowSelection = new Button(window, 0, sideButtonSize, "Flw", onButtonPressed);
	toolbarRight->addComponent(btnFollowSelection);

	btnResetReferenceFrame = new Button(window, 0, sideButtonSize, "RRF", onButtonPressed);
	toolbarRight->addComponent(btnResetReferenceFrame);

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

	FULL_ABOUT_TEXT = "This program is inspired by Yaron Minsky's \"planets\" program.\n\n" + CPLANETS_LICENSE;
	dialogAbout = new DialogBgrWin(Rect(0,0,400,300), "About cplanets");

	btnAboutOk = new Button(dialogAbout, 0, genericButtonSize, "Close", closeDialogBgrWin);
	packLabeledComponent(btnAboutOk);
	setComponentPosition(btnAboutOk, 0.5*(400-btnAboutOk->tw_area.w), 300-btnAboutOk->tw_area.h-WIDGETS_SPACING);

	Rect rectSclpAboutLicense(
			WIDGETS_SPACING,
			dialogAbout->titleBarArea.h + WIDGETS_SPACING,
			400-2*WIDGETS_SPACING,
			300-3*WIDGETS_SPACING-dialogAbout->titleBarArea.h-btnAboutOk->tw_area.h);

	sclpAboutLicense = new ScrollablePane(dialogAbout, Style(0, 14), rectSclpAboutLicense, window->bgcol);
	sclpAboutLicense->content.display_cmd = drawAboutDialog;
	sclpAboutLicense->setScrollbarHorizontalVisible(false);

//	print_h(); //DEBUG
//	cout << "\n" << "Deep analysis:" << endl;
//	WidgetsExtra::print_hierarchy(window);

	//start
	planetarium->start();
	get_events();
	workaround_sdl_stream_file_close(); // part of workaround
}

void onSDLInit()
{
	// workaround to reroute output stream to console
	#ifdef _WIN32
		#ifdef HOTFIX_FOR_SDL_OUTPUT_STREAM_1
			// upon sdl init, reroute streams
			workaround_sdl_stream_file = fopen("CON", "w" );
			freopen( "CON", "w", stdout );
			freopen( "CON", "w", stderr );
		#endif
	#endif

	SDL_Surface* appIcon = SDL_util::loadBitmap("data/icon.bmp", &SDL_util::Color::LIME);
	if(appIcon != null) SDL_WM_SetIcon(appIcon, null);
}

//  ================ CALLBACK DEFINITIONS ================
void draw()
{
	window->clear();
	Point versionStringPos(window->tw_area.w - WIDGETS_SPACING - draw_title_ttf->text_width(VERSION_TEXT.c_str()), window->tw_area.h - WIDGETS_SPACING - TTF_FontHeight(draw_title_ttf->ttf_font));
	draw_title_ttf->draw_string(window->win, VERSION_TEXT.c_str(), versionStringPos);
}

void drawAboutDialog(BgrWin* bw)
{
	BgrWin* dialog = &sclpAboutLicense->content;
	WidgetsExtra::drawBgrWin(bw);

	int logoOffset = 0;
	if(APP_LOGO != null)
	{
		SDL_Rect position = {WIDGETS_SPACING, WIDGETS_SPACING, 0, 0};
		SDL_BlitSurface(APP_LOGO, null, bw->win, &position);
		logoOffset = APP_LOGO->w;
	}

	const int lineHeight = TTF_FontHeight(draw_ttf->ttf_font);

	draw_title_ttf->draw_string(dialog->win, "cplanets, a interactive program to play with gravitation", Point(logoOffset + 3*WIDGETS_SPACING, 0.8*WIDGETS_SPACING));
	draw_title_ttf->draw_string(dialog->win, ("Version " + CPLANETS_VERSION).c_str(), Point(logoOffset + 3*WIDGETS_SPACING, WIDGETS_SPACING + lineHeight));

	const int headerSize = dialogAbout->titleBarArea.h + lineHeight*2;

	int lineNumber = 0;
	foreach(string&, line, vector<string>, (*dialogAboutTextLines))
	{
		draw_ttf->draw_string(dialog->win, line.c_str(), Point(WIDGETS_SPACING, headerSize + lineHeight * lineNumber++));
	}
}

void onWindowResize(int dw, int dh)
{
//	cout << "resize event" << endl;
	if(dialogAbout->parent != null) dialogAbout->hide();

	toolbarNorthLayout->pack();

	planetariumPane->widen(dw, dh);

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
		case SDLK_e:
			if(down) onButtonPressed(btnNew);
			break;
		case SDLK_a:
			if(down) onButtonPressed(btnAddBody);
			break;
		case SDLK_r:
			if(down) onButtonPressed(btnAddRandom);
			aux_isPressed_SDLK_r = down;
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
		adjustAboutDialog();
		setComponentPosition(dialogAbout, window->tw_area.w*0.5 - 200, window->tw_area.h*0.5 - 150);
		dialogAbout->setVisible(dialogAbout->parent==null || dialogAbout->hidden);
	}

	if(btn == btnAddBody)
	{
		planetarium->setBodyCreationMode();
	}

	if(btn == btnAddRandom)
	{
		addRandomBody();
		SDL_CreateThread(keepAddingRandomBodyWhilePressed, null);
	}

	if(btn == btnRemove)
	{
		planetarium->removeFocusedBodies(true);
	}

	if(btn == btnRecolorAll)
	{
		planetarium->recolorAllBodies();
	}

	if(btn == btnFollowSelection)
	{
		const double az = 1/planetarium->viewportZoom;
		planetarium->physics->referenceFrame.set(planetarium->focusedBodies);
		planetarium->viewportPosition.x = -az*planetariumPane->tw_area.w/2;
		planetarium->viewportPosition.y = -az*planetariumPane->tw_area.h/2;
	}

	if(btn == btnResetReferenceFrame)
	{
		const double az = 1/planetarium->viewportZoom;
		planetarium->viewportPosition = planetarium->physics->referenceFrame.position();
		planetarium->physics->referenceFrame.reset();
		planetarium->viewportPosition.x -= az*planetariumPane->tw_area.w/2;
		planetarium->viewportPosition.y -= az*planetariumPane->tw_area.h/2;
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
		planetarium->physics->universe.gravity = DEFAULT_GRAVITY;
		onButtonPressed(btnRun);
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
		if(string(rbtn->label.str) == "Dotted")
		{
			planetarium->orbitTracer.style = Planetarium::OrbitTracer::DOTTED;
			ddmTraceStyle->cmdMenu->src->label = "Dotted";
			ddmTraceStyle->cmdMenu->src->draw_blit_upd();
		}
	}

	if(btn == ddmIntegrationMethod->cmdMenu->buttons && fire)
	{
		RButton* rbtn = btn->act_button();
		if(planetarium->physics->physics2DSolver->factory->solverDisplayName == string(rbtn->label.str))
			return; //avoid doing anything if the chosen integration method is the same as the current

		typedef AbstractPhysics2DSolver::GenericFactory SolverFactory;
		const SolverFactory* selectedSolverFactory = null;
		const_foreach(const SolverFactory*, solverFactory, vector<const SolverFactory*>, AbstractPhysics2DSolver::registeredFactories)
		{
			if(string(rbtn->label.str) == solverFactory->solverDisplayName)
			{
				selectedSolverFactory = solverFactory;
				goto break1;
			}
		}
		break1:

		// todo synchronize this part
		if(selectedSolverFactory != null)
		{
			AbstractPhysics2DSolver* old = planetarium->physics->physics2DSolver;
			planetarium->physics->physics2DSolver = selectedSolverFactory->createSolver(planetarium->physics->universe); //swap solver
			planetarium->physics->physics2DSolver->timeElapsed = old->timeElapsed;
			planetarium->physics->physics2DSolver->timestep = old->timestep;
			spnTimeStep->setValue(&planetarium->physics->physics2DSolver->timestep); //updates the backing value
			delete old;
		}

		ddmIntegrationMethod->cmdMenu->src->label = rbtn->label.str;
		ddmIntegrationMethod->cmdMenu->src->draw_blit_upd();
		spnTimeStep->refresh();
	}
}

void onUserEvent(int cmd,int param,int param2)
{
	if(cmd == PlanetariumPane::USER_EVENT_ID__REDRAW_REQUESTED)
	{
		if(param == planetariumPane->id.id1) //kind of unnecessary, we currently have only one instance of planetarium
		{
			planetariumPane->doRefresh();
		}
	}

	if(cmd == ::USER_EVENT_ID__UPDATE_BODIES_LIST)
	{
		updateSizeTxtBodies();
		sclpBodies->refresh();
	}
}

void onPlanetariumBodyCollision(vector<Body2D>& collidingList, Body2D& resultingMerger)
{
	refreshAllTxtBodies();
}

void onPlanetariumBodyCreation(Body2D& createdBody)
{
	refreshAllTxtBodies();
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
	SDL_WM_SetCaption("cplanets", "cplanets"); //todo maybe put filename on title
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
	txtBodies->setListData(new vector<Body2DClone>(planetarium->getBodies()), true);
	send_uev(::USER_EVENT_ID__UPDATE_BODIES_LIST);
}

void updateSizeTxtBodies()
{
	unsigned height2 = sclpBodies->tw_area.h;

	if(txtBodies->size() >= 0) //if there are texts, make the height of the content to be at least the needed size for the texts
		height2 = Math::max(txtBodies->getListHeight(), (unsigned) sclpBodies->tw_area.h);

	if(height2 != txtBodies->tw_area.h) //avoids unneeded widening
	{
		txtBodies->widen(0, height2 - txtBodies->tw_area.h);
		sclpBodies->widenContent(0, height2 - sclpBodies->content.tw_area.h);
	}
}

void onListSelectionChanged(unsigned ind0, unsigned ind1)
{
//	cout << "I will set the focused bodies as the selected ones in [" << ind0 << ", " << ind1 << "]" << endl;
	sclpBodies->refresh(); //custom redraw behavior

	vector<Body2DClone>& data = *txtBodies->getListData();
	vector<Body2D*> newSelection;

	for(unsigned i = 0; i < data.size(); i++)
		if(txtBodies->selection.isSelected(i))
			newSelection.push_back(data.at(i).original);

	planetarium->setFocusedBodies(newSelection);
}

void onBodyReFocusing()
{
	vector<Body2DClone>& data = *txtBodies->getListData();

	for(unsigned i = 0; i < data.size(); i++)
	{
		txtBodies->selection.modify(i, i, false, true);
		foreach(Body2D*, focused, vector<Body2D*>, planetarium->focusedBodies)
			if(data.at(i).original == focused)
				txtBodies->selection.modify(i, i, true, true);
	}

	sclpBodies->refresh(); //custom redraw behavior
}

void adjustAboutDialog()
{
	if(dialogAboutTextLines == null)
		dialogAboutTextLines = WidgetsExtra::getLineWrappedText(FULL_ABOUT_TEXT, draw_ttf, sclpAboutLicense->content.tw_area.w - 3*WIDGETS_SPACING);

	const int lineHeight = TTF_FontHeight(draw_ttf->ttf_font);

	const int headerSize = dialogAbout->titleBarArea.h + lineHeight*2;
	const int totalSize = headerSize + lineHeight * dialogAboutTextLines->size();

	//expand BgrWin to fit the text
	if(totalSize > sclpAboutLicense->content.tw_area.h)
	{
		sclpAboutLicense->widenContent(0, totalSize - sclpAboutLicense->content.tw_area.h);
		sclpAboutLicense->refresh();
	}
}

void closeDialogBgrWin(Button* btn)
{
	DialogBgrWin* self = static_cast<DialogBgrWin*>(btn->parent);
	self->setVisible(false);
	if(self->onClosedCallback != null)
		self->onClosedCallback(self);
}

void replaceUniverse(Universe2D* universe)
{
	onButtonPressed(btnPause);
	planetarium->setUniverse(universe);
	refreshAllTxtBodies();
	spnTimeStep->setValue(&(planetarium->physics->physics2DSolver->timestep));
}

void addRandomBody()
{
	double az = 1/planetarium->viewportZoom;
	double diameter = (planetarium->bodyCreationDiameterRatio * az) * Planetarium::BODY_CREATION_DIAMETER_FACTOR;
	double mass = (Math::PI/6.0) * planetarium->bodyCreationDensity * diameter * diameter * diameter;
	double speed = *spnBodyVelocity->getValue();
	Vector2D randomPosition(randomBetween(0, planetariumPane->tw_area.w), randomBetween(0, planetariumPane->tw_area.h));
	Vector2D randomVelocity(randomBetween(-speed * az, speed * az), randomBetween(-speed * az, speed * az));
	randomPosition.scale(az).add(planetarium->viewportPosition);

	//TODO set velocity to orbit: ve = sqrt(2GM/r) when adding orbiting body
	planetarium->addCustomBody(new Body2D(mass, diameter, randomPosition, randomVelocity, Vector2D::NULL_VECTOR), SDL_util::getRandomColor());
}

int keepAddingRandomBodyWhilePressed(void* unused)
{
	SDL_Delay(500);
	long lastUpdateTime;
	while(aux_isPressed_SDLK_r)
	{
		lastUpdateTime = SDL_GetTicks();
		addRandomBody();
		SDL_Delay(5000/planetarium->fps - (SDL_GetTicks() - lastUpdateTime));
	}
	return 0;
}
