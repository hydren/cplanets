/*
 * main_window.cpp
 *
 *  Created on: 8 de jan de 2016
 *      Author: professor
 */
#include <ciso646>

#include "main_window.hpp"

#include "hotfixes.h" //macros to enable/disable hotfixes for SDL-related issues

#include <cmath>
#include <cstring>
#include <climits>

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include "futil/math/more_random.h"
#include "futil/string/callbacks.hpp"

#include "SDL_widgets/SDL_widgets.h"
#include "SDL_util.hpp"

#include "planetarium/planetarium_pane.hpp"
#include "program_io.hpp"

#include "widgets/widgets_util.hpp"
#include "widgets/widgets_debug.hpp"
#include "widgets/flow_layout.hpp"
#include "widgets/spinner.hpp"
#include "widgets/drop_menu.hpp"
#include "widgets/tabs.hpp"
#include "widgets/label_win.hpp"
#include "widgets/toogle_button.hpp"
#include "widgets/file_dialog.hpp"
#include "widgets/scrollable_pane.hpp"
#include "widgets/list_win.hpp"
#include "widgets/list_selection_model_extra.hpp"
#include "widgets/value_shower.hpp"
#include "widgets/icon_button.hpp"
#include "widgets/multi_line_text_renderer.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;

using WidgetsExtra::packLabeledComponent;
using WidgetsExtra::setComponentPosition;
using WidgetsExtra::LabeledComponentPacker;
using WidgetsExtra::Layout;
using WidgetsExtra::FlowLayout;
using WidgetsExtra::Spinner;
using WidgetsExtra::DropDownMenu;
using WidgetsExtra::DropDownMenuFactory;
using WidgetsExtra::LabelWin;
using WidgetsExtra::ToogleButton;
using WidgetsExtra::FileDialog;
using WidgetsExtra::ScrollablePane;
using WidgetsExtra::DialogBgrWin;
using WidgetsExtra::ListWin;
using WidgetsExtra::ValueShower;
using WidgetsExtra::IconButton;
using WidgetsExtra::IconToogleButton;
using WidgetsExtra::MultiLineTextRenderer;
using WidgetsExtra::TabbedPane;

typedef Planetarium::Body2DClone Body2DClone;

#define max std::max

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
void onSDLInit();

void draw(); // The drawing function.
void drawAboutDialog(BgrWin* dialog);
void drawHelpDialog(BgrWin* dialog);
void drawPlanetariumWithVersion(BgrWin* bgr);

// widgets calbacks
void onWindowResize(int dw, int dh); // callback for window resizing events
void onKeyEvent(SDL_keysym *key,bool down);
void onButtonPressed(Button* btn);
void onCheckBoxPressed(CheckBox* chck);
void onCheckBoxPressed(CheckBox* chck, bool fake);
void onDropDownMenuButton(RButWin*,int nr,int fire);

void onFileChosenOpenUniverse(FileDialog* dialog);
void onFileChosenSaveUniverse(FileDialog* dialog);

void onUserEvent(int cmd,int param,int param2);

// planetarium callbacks
void onListSelectionChanged(unsigned, unsigned);
void onBodyReFocus();
void onBodyDeletion(Body2D* deletedBodyPtr);

// actions
void txtBodiesRefreshAll();
void txtBodiesUpdateSize();

void dialogAboutAdjust();
void collapseTabs(bool choice);
void toogleFullscreen(bool choice);

void loadUniverseFromFile(const string& path);
void replaceUniverse(const Universe2D& universeCopy);
void addRandomBody(bool orbiting=false);

// thread functions
int keepAddingRandomBodyWhilePressed(void* unused);

// ============= AUX. FUNCTION ================

SDL_Surface* loadImage(const char* path, const SDL_Color* transparentColor=null)
{
	SDL_Surface* bmp = IMG_Load(path);
	if(bmp != null && transparentColor != null)
		SDL_SetColorKey(bmp, SDL_SRCCOLORKEY, SDL_MapRGB(bmp->format, transparentColor->r, transparentColor->g, transparentColor->b));

	return bmp;
}

SDL_Surface* resizeImage(SDL_Surface* surf, unsigned width, bool alsoFreeArg=false)
{
	if(surf == null) return null;
	SDL_Surface* resized = rotozoomSurface(surf, 0, width/ (double) surf->w, true);
	if(alsoFreeArg) SDL_FreeSurface(surf);
	return resized;
}

//  =========== PLANETARIUM LISTENER ===========================

struct CustomListener extends Planetarium::UniverseEventListener, WidgetsExtra::ListSelectionModel::Listener
{
	void onBodyCollision(vector<Body2D>& collidingList, Body2D& resultingMerger) { txtBodiesRefreshAll(); }
	void onBodyCreation(Body2D& createdBody) { txtBodiesRefreshAll(); }
	void onBodyDeletion(Body2D* deletedBody) { ::onBodyDeletion(deletedBody); }
	void onBodyReFocus(){ ::onBodyReFocus(); }
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

#include "help.h"

//  ================ VARIABLES ===============
Rect genericButtonSize(0, 0, TOOLBAR_SIZE, TOOLBAR_SIZE); //useful to reuse
Rect genericToolbarButtonSize(0, 0, TOOLBAR_SIZE-2*WIDGETS_SPACING, TOOLBAR_SIZE-2*WIDGETS_SPACING);
CustomListener customListener;
RenderText* draw_light_ttf;
bool aux_isPressed_SDLK_r = false;

//  ================ THEMES =================
#include "themes.hxx"
string colorThemeName;

//  ================ COMPONENTS ================
TopWin* window; // The program window
Rect windowSize(0, 0, 640, 480);

FlowLayout* toolbarNorthLayout;
IconButton* btnNew, *btnLoad, *btnSave, *btnUndo, *btnRewind, *btnHelp, *btnAbout;
ToogleButton* tglFullScreen, *tglCollapseLeftPanel;
IconButton* btnRun, *btnPause;

TabbedPane* tabs;

BgrWin* tabBodies;
ListWin<Body2DClone>* txtBodies;
ScrollablePane* sclpBodies;

BgrWin* tabOptions;
LabelWin* lblSimulationParameters;
DropDownMenu* ddmIntegrationMethod;
Spinner<double>* spnGravity, *spnGExp;
Spinner<short>* spnFPS;
Spinner<double>* spnTimeStep;
Spinner<long>* spnStepDelay;
CheckBox* chckLegacyParameters;
Spinner<long>* spnDisplayPeriod, *spnIterPerDisplay;
CheckBox* chckBouncingOnCollision, *chckComputeRocheLimit;
LabelWin* lblBodyCreation;
Spinner<double>* spnBodyDiameter, *spnBodyDensity, *spnBodyVelocity;
LabelWin* lblOrbitTracing;
CheckBox* chckTraceOrbit;
Spinner<unsigned>* spnTraceLength;
DropDownMenu* ddmTraceStyle;

PlanetariumPane* planetariumPane;
Planetarium* planetarium; ///helper pointer

FlowLayout* toolbarRight;
IconButton* btnAddBody, *btnAddRandom, *btnAddRandomOrbiting, *btnRemove, *btnClear,
	*btnRecolorAll, *btnFollowSelection, *btnResetReferenceFrame;

FlowLayout* toolbarSouthLayout;
IconToogleButton* tgbTraceOrbit;
IconButton* btnDoubleTraceLength, *btnHalveTraceLentgh;
IconToogleButton* tgbAA;
ValueShower* msgBodyCount;
CheckBox* chckComputeEnergy;
ValueShower* msgLogK, *msgLogP, *msgLogE;

FileDialog* dialogLoad, *dialogSave;

DialogBgrWin* dialogAbout;
Button* btnAboutOk;
ScrollablePane* sclpAboutLicense;
MultiLineTextRenderer* mltAboutText;

DialogBgrWin* dialogHelp;
ScrollablePane* sclpHelpText;
MultiLineTextRenderer* mltHelpText;

// kludges

void forceFullWindowRefresh()
{
	window->draw_blit_recur();
	SDL_Flip(window->win);
}

// ================ CPlanetsGUI::MainWindow namespace ================
string filePathToLoad;
#include "cli.hxx"

void CPlanets::init()
{
	window = new TopWin("cplanets", windowSize, SDL_INIT_VIDEO, SDL_RESIZABLE, draw, null, onSDLInit);
	handle_rsev = onWindowResize; //set callback for window resize
	handle_kev = onKeyEvent; //set callback for keyboard events
	handle_uev = onUserEvent;
	VERSION_TEXT = string("v").append(CPLANETS_VERSION);
	APP_LOGO = resizeImage(loadImage("data/icon.png"), 40, true);
	SDL_Color colorLight = {192, 192, 192,  0};
	draw_light_ttf = new RenderText(draw_title_ttf->ttf_font, colorLight);

	const Theme& theme = Theme::getThemeByName(colorThemeName).init();

	window->bgcol = theme.bgcol;

	//+++++++++++++++ North toolbar
	toolbarNorthLayout = new FlowLayout(WIDGETS_SPACING, WIDGETS_SPACING);

	btnNew = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/new.png"), onButtonPressed);
	toolbarNorthLayout->addComponent(btnNew);

	btnLoad = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/load.png"), onButtonPressed);
	toolbarNorthLayout->addComponent(btnLoad);

	btnSave = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/save.png"), onButtonPressed);
	toolbarNorthLayout->addComponent(btnSave);

	toolbarNorthLayout->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::HORIZONTAL, TOOLBAR_SIZE)));
	toolbarNorthLayout->getComponentAt(toolbarNorthLayout->getComponentCount()-1)->offset.y = -5;

	btnUndo = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/undo.png"), onButtonPressed);
	toolbarNorthLayout->addComponent(btnUndo);

	btnRewind = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/rewind.png"), onButtonPressed);
	toolbarNorthLayout->addComponent(btnRewind);

	toolbarNorthLayout->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::HORIZONTAL, TOOLBAR_SIZE)));
	toolbarNorthLayout->getComponentAt(toolbarNorthLayout->getComponentCount()-1)->offset.y = -5;

//	btnHelp = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/help.png"), onButtonPressed);
//	toolbarNorthLayout->addComponent(btnHelp);

	btnAbout = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/about.png"), onButtonPressed);
	toolbarNorthLayout->addComponent(btnAbout);

	toolbarNorthLayout->addComponent(new Layout::Spacer(toolbarNorthLayout));

	tglFullScreen = new IconToogleButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/fullscreen.png"), onCheckBoxPressed);
	toolbarNorthLayout->addComponent(tglFullScreen);

	tglCollapseLeftPanel = new IconToogleButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/collapse-tabs.png"), onCheckBoxPressed);
	toolbarNorthLayout->addComponent(tglCollapseLeftPanel);

	toolbarNorthLayout->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::HORIZONTAL, TOOLBAR_SIZE)));
	toolbarNorthLayout->getComponentAt(toolbarNorthLayout->getComponentCount()-1)->offset.y = -5;

	btnRun = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/run.png"), onButtonPressed);
	toolbarNorthLayout->addComponent(btnRun);

	btnPause = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/pause.png"), onButtonPressed);
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
	planetariumPane->display_cmd = drawPlanetariumWithVersion;
	planetarium = planetariumPane->planetarium;
	if(not filePathToLoad.empty()) try { loadUniverseFromFile(filePathToLoad); }
	catch (std::exception& e) { cout << "A problem occurred when loading file. " << e.what() << endl; }
	planetarium->listeners.addListener(&customListener);
	planetarium->physics->universe.gravity = DEFAULT_GRAVITY;

	//+++++++++++++++ Tabs
	tabs = new TabbedPane(window, Rect(WIDGETS_SPACING, TOOLBAR_SIZE + 0.5*WIDGETS_SPACING, BODIES_PANEL_WIDTH - WIDGETS_SPACING, planetariumSize.h));
	Rect sizeTab(
		0,
		tabs->tabBtnLayout.maxHeight,
		tabs->tw_area.w,
		tabs->tw_area.h - tabs->tabBtnLayout.maxHeight
	);

	// Tab bodies
	tabBodies = new BgrWin(tabs, sizeTab, null, TabbedPane::drawTabStyleBgrWin, null, null, null, tabs->bgcol);
	tabs->addTab("Bodies", tabBodies);

	sclpBodies = new ScrollablePane(tabBodies, theme.scrollStyle, Rect(2, 2, sizeTab.w - 3, sizeTab.h - 3), tabs->bgcol);
	sclpBodies->setScrollbarHorizontalVisible(false);

	Rect txtBodiesSize(0, 0, sclpBodies->tw_area.w, sclpBodies->tw_area.h);
	txtBodies = new ListWin<Body2DClone>(&sclpBodies->content, theme.listWinStyle, txtBodiesSize, stringfy_by_method<Body2DClone, &Body2DClone::toString>);
	txtBodies->setListData(new vector<Body2DClone>(planetarium->getBodies()), true);
	txtBodies->selectionAdjustmentFunction = WidgetsExtra::GenericSelectionAdjustment::function<Body2DClone>;
	txtBodies->selection.listenerAdd(&customListener);
	txtBodies->selection.onChange = onListSelectionChanged;
	txtBodies->preventRedrawOnClick = true;

	// Tab options
	tabOptions = new BgrWin(tabs, sizeTab, null, TabbedPane::drawTabStyleBgrWin, null, null, null, tabs->bgcol);
	tabs->addTab("Options", tabOptions);

	tabs->tabBtnLayout.pack();
	tabs->setActiveTab(tabBodies);

	lblSimulationParameters = new LabelWin(tabOptions, Rect(), "Simulation parameters");
	lblSimulationParameters->setTextRenderer(draw_title_ttf);
	setComponentPosition(lblSimulationParameters, WIDGETS_SPACING, WIDGETS_SPACING);

	DropDownMenuFactory factory;
	factory.setAppearance(DropDownMenuFactory::COMBOBOX);
	factory.setCallback(onDropDownMenuButton);

	factory.setLabel("Integration method: ", true);
	factory.setSize(Rect(40, 40, 200, 20));
	typedef AbstractPhysics2DSolver::GenericFactory SolverFactory;
	const_foreach(const SolverFactory*, solverFactory, vector<const SolverFactory*>, AbstractPhysics2DSolver::registeredFactories)
	{
		factory.addItem(solverFactory->solverDisplayName.c_str());
	}
	ddmIntegrationMethod = factory.createAt(tabOptions);
	ddmIntegrationMethod->cmdMenu->src->label = Label(planetarium->physics->solver->factory->solverDisplayName.c_str());
	ddmIntegrationMethod->setPosition(Point(lblSimulationParameters->area.x, lblSimulationParameters->area.y + lblSimulationParameters->tw_area.h + WIDGETS_SPACING));
	ddmIntegrationMethod->offset.y = -10;

	spnGravity = new Spinner<double>(tabOptions, theme.buttonStyle, Rect(0,0,2.4*TOOLBAR_SIZE, TOOLBAR_SIZE), "Gravity:");
	setComponentPosition(spnGravity, ddmIntegrationMethod->getPosition().x, ddmIntegrationMethod->getPosition().y + ddmIntegrationMethod->getSize().h + WIDGETS_SPACING);
	spnGravity->setValue(&(planetarium->physics->universe.gravity), true);
	spnGravity->setStepValue(0.1);

	spnGExp = new Spinner<double>(tabOptions, theme.buttonStyle, Rect(0,0,1.8*TOOLBAR_SIZE, TOOLBAR_SIZE), "G. Exp:");
	setComponentPosition(spnGExp, spnGravity->area.x + spnGravity->tw_area.w + WIDGETS_SPACING, spnGravity->area.y);
	spnGExp->setValue(&(planetarium->physics->universe.gExp), true);
	spnGExp->setStepValue(0.1);

	spnFPS = new Spinner<short>(tabOptions, theme.buttonStyle, Rect(0,0,1.8*TOOLBAR_SIZE, TOOLBAR_SIZE), "FPS:");
	setComponentPosition(spnFPS, spnGExp->area.x + spnGExp->tw_area.w + WIDGETS_SPACING, spnGExp->area.y);
	spnFPS->setValue(&(planetarium->fps), true);

	spnTimeStep = new Spinner<double>(tabOptions, theme.buttonStyle, Rect(0,0,2.4*TOOLBAR_SIZE, TOOLBAR_SIZE), "Time step:");
	setComponentPosition(spnTimeStep, spnGravity->area.x, spnGravity->area.y + spnGravity->tw_area.h + WIDGETS_SPACING);
	spnTimeStep->setValue(&(planetarium->physics->solver->timestep), true);
	spnTimeStep->setStepValue(0.1);

	spnStepDelay = new Spinner<long>(tabOptions, theme.buttonStyle, Rect(0,0,2.5*TOOLBAR_SIZE, TOOLBAR_SIZE), "Step delay:");
	setComponentPosition(spnStepDelay, spnTimeStep->area.x + spnTimeStep->tw_area.w + WIDGETS_SPACING, spnTimeStep->area.y);
	spnStepDelay->setValue(&(planetarium->stepDelay), true);

	chckLegacyParameters = new CheckBox(tabOptions, 0, genericButtonSize, "Use legacy parameters:", onCheckBoxPressed);
	chckLegacyParameters->d = &(planetarium->legacyControl);  // binds the checkbox to the variable
	setComponentPosition(chckLegacyParameters, spnTimeStep->area.x, spnTimeStep->area.y + spnTimeStep->tw_area.h + WIDGETS_SPACING);
	packLabeledComponent(chckLegacyParameters);

	spnDisplayPeriod = new Spinner<long>(tabOptions, theme.buttonStyle, Rect(0,0,3.6*TOOLBAR_SIZE, TOOLBAR_SIZE), "Disp. period (ms):");
	setComponentPosition(spnDisplayPeriod, chckLegacyParameters->area.x, chckLegacyParameters->area.y + chckLegacyParameters->tw_area.h + WIDGETS_SPACING);
	spnDisplayPeriod->setValue(&(planetarium->displayPeriod), true);

	spnIterPerDisplay = new Spinner<long>(tabOptions, theme.buttonStyle, Rect(0,0,2.8*TOOLBAR_SIZE, TOOLBAR_SIZE), "Iter/display:");
	setComponentPosition(spnIterPerDisplay, spnDisplayPeriod->area.x + spnDisplayPeriod->tw_area.w + WIDGETS_SPACING, spnDisplayPeriod->area.y);
	spnIterPerDisplay->setValue(&(planetarium->iterationsPerDisplay), true);

	chckBouncingOnCollision = new CheckBox(tabOptions, 0, genericButtonSize, "Bounce on collision (experimental)", onCheckBoxPressed);
	setComponentPosition(chckBouncingOnCollision, spnDisplayPeriod->area.x, spnDisplayPeriod->area.y + spnDisplayPeriod->tw_area.h + WIDGETS_SPACING);
	packLabeledComponent(chckBouncingOnCollision);

	chckComputeRocheLimit = new CheckBox(tabOptions, 0, genericButtonSize, "Highlight bodies within roche limit", onCheckBoxPressed);
	chckComputeRocheLimit->d = &(planetarium->rocheLimitComputingEnabled);
	setComponentPosition(chckComputeRocheLimit, chckBouncingOnCollision->area.x, chckBouncingOnCollision->area.y + chckBouncingOnCollision->tw_area.h + WIDGETS_SPACING);
	packLabeledComponent(chckComputeRocheLimit);

	lblBodyCreation = new LabelWin(tabOptions, Rect(), "Body creation parameters");
	lblBodyCreation->setTextRenderer(draw_title_ttf);
	setComponentPosition(lblBodyCreation, chckComputeRocheLimit->area.x, chckComputeRocheLimit->area.y + chckComputeRocheLimit->tw_area.h + 1.8*WIDGETS_SPACING);

	spnBodyDiameter = new Spinner<double>(tabOptions, theme.buttonStyle, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Diameter:");
	setComponentPosition(spnBodyDiameter, lblBodyCreation->area.x, lblBodyCreation->area.y + lblBodyCreation->tw_area.h + WIDGETS_SPACING);
	spnBodyDiameter->setValue(&(planetarium->bodyCreationDiameterRatio), true);
	spnBodyDiameter->setStepValue(0.1);

	spnBodyDensity = new Spinner<double>(tabOptions, theme.buttonStyle, Rect(0,0,2.3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Density:");
	setComponentPosition(spnBodyDensity, spnBodyDiameter->area.x + spnBodyDiameter->tw_area.w + WIDGETS_SPACING, spnBodyDiameter->area.y);
	spnBodyDensity->setValue(&(planetarium->bodyCreationDensity), true);
	spnBodyDensity->setStepValue(0.1);

	spnBodyVelocity = new Spinner<double>(tabOptions, theme.buttonStyle, Rect(0,0,5.75*TOOLBAR_SIZE, TOOLBAR_SIZE), "Velocity (for random objects):");
	setComponentPosition(spnBodyVelocity, spnBodyDiameter->area.x, spnBodyDiameter->area.y + spnBodyDiameter->tw_area.h + WIDGETS_SPACING);
	spnBodyVelocity->setValue(&(planetarium->bodyCreationSpeed), true);
	spnBodyVelocity->setStepValue(0.1);

	lblOrbitTracing = new LabelWin(tabOptions, Rect(), "Orbit tracing");
	lblOrbitTracing->setTextRenderer(draw_title_ttf);
	setComponentPosition(lblOrbitTracing, spnBodyVelocity->area.x, spnBodyVelocity->area.y + spnBodyVelocity->tw_area.h + 1.6*WIDGETS_SPACING);

	chckTraceOrbit = new CheckBox(tabOptions, 0, genericButtonSize, "Show orbit trace", onCheckBoxPressed);
	chckTraceOrbit->d = &(planetarium->getOrbitTracerSwitchReference());  // binds the checkbox to the variable
	setComponentPosition(chckTraceOrbit, lblOrbitTracing->area.x, lblOrbitTracing->area.y + lblOrbitTracing->tw_area.h + 1.5*WIDGETS_SPACING);
	packLabeledComponent(chckTraceOrbit);

	spnTraceLength = new Spinner<unsigned>(tabOptions, theme.buttonStyle, Rect(0, 0, 3*TOOLBAR_SIZE, TOOLBAR_SIZE), "Trace length:");
	setComponentPosition(spnTraceLength, chckTraceOrbit->area.x, chckTraceOrbit->area.y + chckTraceOrbit->tw_area.h + 0.5*WIDGETS_SPACING + 3);
	spnTraceLength->setValue(&(planetarium->getOrbitTracerLengthReference()), true);

	factory.removeAllItems();
	factory.setLabel("Trace style: ", true);
	factory.setSize(Rect(40, 40, 100, 20));
	factory.addItem("Linear");
	factory.addItem("Dotted");
	ddmTraceStyle = factory.createAt(tabOptions);
	ddmTraceStyle->setPosition(Point(spnTraceLength->area.x + spnTraceLength->tw_area.w + WIDGETS_SPACING, spnTraceLength->area.y - 3));
	ddmTraceStyle->offset.y = -10;

	//+++++++++++++++ East (right) toolbar
	toolbarRight = new FlowLayout(planetariumSize.x + planetariumSize.w + WIDGETS_SPACING*1.5, TOOLBAR_SIZE + 0.5*WIDGETS_SPACING);
	toolbarRight->orientation = Layout::VERTICAL;

	toolbarRight->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::VERTICAL, TOOLBAR_SIZE)));
	toolbarRight->getComponentAt(toolbarRight->getComponentCount()-1)->offset.x = -5;

	btnAddBody = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/add.png"), onButtonPressed);
	toolbarRight->addComponent(btnAddBody);

	btnAddRandom = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/add-random.png"), onButtonPressed);
	toolbarRight->addComponent(btnAddRandom);

	btnAddRandomOrbiting = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/add-orbiting.png"), onButtonPressed);
	toolbarRight->addComponent(btnAddRandomOrbiting);

	btnRemove = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/remove.png"), onButtonPressed);
	toolbarRight->addComponent(btnRemove);

	btnClear = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/clear.png"), onButtonPressed);
	toolbarRight->addComponent(btnClear);

	toolbarRight->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::VERTICAL, TOOLBAR_SIZE)));
	toolbarRight->getComponentAt(toolbarRight->getComponentCount()-1)->offset.x = -5;

	btnRecolorAll = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/recolor.png"), onButtonPressed);
	toolbarRight->addComponent(btnRecolorAll);

	toolbarRight->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::VERTICAL, TOOLBAR_SIZE)));
	toolbarRight->getComponentAt(toolbarRight->getComponentCount()-1)->offset.x = -5;

	btnFollowSelection = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/follow.png"), onButtonPressed);
	toolbarRight->addComponent(btnFollowSelection);

	btnResetReferenceFrame = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/reset-frame.png"), onButtonPressed);
	toolbarRight->addComponent(btnResetReferenceFrame);

	toolbarRight->pack();

	//+++++++++++++++ South toolbar
	toolbarSouthLayout = new FlowLayout(WIDGETS_SPACING, windowSize.h - (1.25*TOOLBAR_SIZE - 2*WIDGETS_SPACING));
	toolbarSouthLayout->alignment = FlowLayout::MIDDLE;

	tgbTraceOrbit = new IconToogleButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/trace-orbit.png"), onCheckBoxPressed);
	tgbTraceOrbit->d = &(planetarium->getOrbitTracerSwitchReference());  // binds the checkbox to the variable
	toolbarSouthLayout->addComponent(tgbTraceOrbit);

	btnDoubleTraceLength = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/trace-double.png"), onButtonPressed);
	toolbarSouthLayout->addComponent(btnDoubleTraceLength);

	btnHalveTraceLentgh = new IconButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/trace-half.png"), onButtonPressed);
	toolbarSouthLayout->addComponent(btnHalveTraceLentgh);

	toolbarSouthLayout->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::HORIZONTAL, TOOLBAR_SIZE)));

	tgbAA = new IconToogleButton(window, theme.toolbarButtonStyle, genericToolbarButtonSize, Label(""), loadImage("data/smooth.png"), onCheckBoxPressed);
	tgbAA->d = &(planetarium->tryAA);
	toolbarSouthLayout->addComponent(tgbAA);

	toolbarSouthLayout->addComponent(new Layout::Spacer(toolbarNorthLayout));
	toolbarSouthLayout->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::HORIZONTAL, TOOLBAR_SIZE)));

	msgBodyCount = new ValueShower(window, Style(2), Point(), 8, "Count:");
	toolbarSouthLayout->addComponent(msgBodyCount);
	toolbarSouthLayout->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::HORIZONTAL, TOOLBAR_SIZE)));

	chckComputeEnergy = new CheckBox(window, 0, genericButtonSize, "", onCheckBoxPressed);
	chckComputeEnergy->d = &(planetarium->physics->systemEnergyComputingEnabled);  // binds the checkbox to the variable
	packLabeledComponent(chckComputeEnergy);
	toolbarSouthLayout->addComponent(chckComputeEnergy);

	msgLogK = new ValueShower(window, Style(2), Point(), 10, "Log K.:");
	toolbarSouthLayout->addComponent(msgLogK);

	toolbarSouthLayout->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::HORIZONTAL, TOOLBAR_SIZE)));

	msgLogP = new ValueShower(window, Style(2), Point(), 10, "Log P.:");
	toolbarSouthLayout->addComponent(msgLogP);

	toolbarSouthLayout->addComponent(static_cast<Layout::Element*>(new Layout::Separator(window, Layout::HORIZONTAL, TOOLBAR_SIZE)));

	msgLogE = new ValueShower(window, Style(2), Point(), 10, "Log E.:");
	toolbarSouthLayout->addComponent(msgLogE);

	toolbarSouthLayout->pack();

	vector<string> strFiletypes;
	strFiletypes.push_back("Plain text (*.txt)");
	dialogLoad = new FileDialog(FileDialog::SELECT_FILE, onFileChosenOpenUniverse, strFiletypes, theme.dialogStyle, theme.buttonStyle);
	dialogSave = new FileDialog(FileDialog::SAVE_FILE, onFileChosenSaveUniverse, strFiletypes, theme.dialogStyle, theme.buttonStyle);

	FULL_ABOUT_TEXT = string("This program is inspired by Yaron Minsky's \"planets\" program.\n\n").append(CPLANETS_LICENSE);
	dialogAbout = new DialogBgrWin(Rect(0,0,400,300), "About cplanets", null, theme.dialogStyle);

	btnAboutOk = new Button(dialogAbout, Style(0, 1), genericButtonSize, "Close", DialogBgrWin::buttonCallbackCloseParentDialogBgrWin);
	packLabeledComponent(btnAboutOk);
	setComponentPosition(btnAboutOk, 0.5*(400-btnAboutOk->tw_area.w), 300-btnAboutOk->tw_area.h-WIDGETS_SPACING);

	Rect rectSclpAboutLicense(
		WIDGETS_SPACING,
		dialogAbout->titleBarArea.h + WIDGETS_SPACING,
		400-2*WIDGETS_SPACING,
		300-3*WIDGETS_SPACING-dialogAbout->titleBarArea.h-btnAboutOk->tw_area.h);

	sclpAboutLicense = new ScrollablePane(dialogAbout, theme.scrollStyle, rectSclpAboutLicense, window->bgcol);
	sclpAboutLicense->content.display_cmd = drawAboutDialog;
	sclpAboutLicense->setScrollbarHorizontalVisible(false);

	Point posMltAboutText(WIDGETS_SPACING, dialogAbout->titleBarArea.h + TTF_FontHeight(draw_title_ttf->ttf_font)*2);
	mltAboutText = new MultiLineTextRenderer(draw_ttf, null, posMltAboutText, 3*WIDGETS_SPACING);
	mltAboutText->setText(FULL_ABOUT_TEXT, sclpAboutLicense->content.tw_area.w);

	dialogHelp = new DialogBgrWin(Rect(0,0,400,300), "Help", null, theme.dialogStyle);

	Rect rectSclpHelpText(
		WIDGETS_SPACING,
		WIDGETS_SPACING + dialogHelp->titleBarArea.h,
		dialogHelp->tw_area.w - 2*WIDGETS_SPACING,
		dialogHelp->tw_area.h - 3*WIDGETS_SPACING - dialogHelp->titleBarArea.h);
	sclpHelpText = new ScrollablePane(dialogHelp, theme.scrollStyle, rectSclpHelpText, window->bgcol);
	sclpHelpText->content.display_cmd = drawHelpDialog;
	sclpHelpText->setScrollbarHorizontalVisible(false);

	mltHelpText = new MultiLineTextRenderer(draw_ttf, null, Point(), 3*WIDGETS_SPACING);
	mltHelpText->setText(HELP_TEXT, sclpHelpText->tw_area.w);

//	cout << "\n" << "Basic analysis:" << endl;
//	print_h();  // DEBUG
//	cout << "\n" << "Deep analysis:" << endl;
//	WidgetsExtra::print_hierarchy(window);  // DEBUG
}

void CPlanets::start()
{
	//start
	planetarium->start();
	get_events();
	workaround_sdl_stream_file_close(); // part of workaround
}

//  ================ CALLBACK DEFINITIONS ================
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
	if(appIcon != null)
	{
		SDL_WM_SetIcon(appIcon, null);
		SDL_FreeSurface(appIcon); //appIcon was copied, we don't need it anymore
	}
}

void draw()
{
	window->clear();
	//Point versionStringPos(window->tw_area.w - WIDGETS_SPACING - draw_title_ttf->text_width(VERSION_TEXT.c_str()), window->tw_area.h - WIDGETS_SPACING - TTF_FontHeight(draw_title_ttf->ttf_font));
	//draw_title_ttf->draw_string(window->win, VERSION_TEXT.c_str(), versionStringPos);

	msgBodyCount->draw_label();
	msgLogK->draw_label();
	msgLogP->draw_label();
	msgLogE->draw_label();
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

	static const string versionStr = string("Version ").append(CPLANETS_VERSION), titleStr("cplanets, a interactive program to play with gravitation");
	draw_title_ttf->draw_string(dialog->win, titleStr.c_str()  , Point(logoOffset + 3*WIDGETS_SPACING, 2.5*WIDGETS_SPACING));
	draw_title_ttf->draw_string(dialog->win, versionStr.c_str(), Point(logoOffset + 3*WIDGETS_SPACING, 2.5*WIDGETS_SPACING + TTF_FontHeight(draw_title_ttf->ttf_font)));

	mltAboutText->draw(dialog->win);
}

void drawHelpDialog(BgrWin* bw)
{
	BgrWin* dialog = &sclpHelpText->content;
	WidgetsExtra::drawBgrWin(bw);
	mltHelpText->draw(dialog->win);
}

void drawPlanetariumWithVersion(BgrWin* bgr)
{
	static const string label = string("v") + CPLANETS_VERSION;
	const Point labelPos(bgr->tw_area.w - draw_light_ttf->text_width(label.c_str()), bgr->tw_area.h - TTF_FontHeight(draw_light_ttf->ttf_font));
	PlanetariumPane::drawPlanetarium(bgr);
	draw_light_ttf->draw_string(bgr->win, label.c_str(), labelPos);
}

void onWindowResize(int dw, int dh)
{
//	cout << "resize event" << endl;
	if(dialogAbout->parent != null) dialogAbout->hide();
	if(*tglCollapseLeftPanel->d == true) collapseTabs(false);

	toolbarNorthLayout->pack();

	planetariumPane->widen(dw, dh);

	tabs->widen(0, dh);
	sclpBodies->widen(0, dh);
	txtBodiesUpdateSize();

	toolbarRight->position.x += dw;
	toolbarRight->pack();

	toolbarSouthLayout->position.y += dh;
	toolbarSouthLayout->pack();

	window->draw_blit_recur();
	if(*tglCollapseLeftPanel->d == true) collapseTabs(true);
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
		case SDLK_EQUALS:
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
			if(down) onButtonPressed(btnClear);
			break;
		case SDLK_a:
			if(down) onButtonPressed(btnAddBody);
			break;
		case SDLK_r:
			if(down) onButtonPressed(btnAddRandom);
			aux_isPressed_SDLK_r = down;
			break;
		case SDLK_j:
			if(down) onButtonPressed(btnAddRandomOrbiting);
			aux_isPressed_SDLK_r = down;
			break;
		case SDLK_o:
			if(down) onButtonPressed(btnRecolorAll);
			break;
		case SDLK_u:
			if(down) onButtonPressed(btnUndo);
			break;
		case SDLK_g:
			if(down) onButtonPressed(btnRewind);
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
		case SDLK_SPACE:
			if(down)
			{
				planetarium->setFocusedBodies(planetarium->physics->universe.bodies);
				onButtonPressed(btnFollowSelection);
			}
			break;
		case SDLK_ESCAPE:
			if(down and *tglFullScreen->d == true)
			{
				onCheckBoxPressed(tglFullScreen, true);
			}
			break;
		default:break;
	}
}

void onButtonPressed(Button* btn)
{
	if(btn == btnAbout)
	{
		dialogAboutAdjust();
		setComponentPosition(dialogAbout, window->tw_area.w*0.5 - 200, window->tw_area.h*0.5 - 150);
		dialogAbout->setVisible(dialogAbout->parent==null or dialogAbout->hidden);
	}

	if(btn == btnHelp)
	{
		setComponentPosition(dialogHelp, window->tw_area.w*0.5 - 200, window->tw_area.h*0.5 - 150);
		dialogHelp->setVisible(dialogHelp->parent==null or dialogHelp->hidden);
	}

	if(btn == btnAddBody)
	{
		planetarium->setBodyCreationMode();
	}

	if(btn == btnAddRandom)
	{
		addRandomBody();
		SDL_CreateThread(keepAddingRandomBodyWhilePressed, new bool(false));
	}

	if(btn == btnAddRandomOrbiting)
	{
		addRandomBody(true);
		SDL_CreateThread(keepAddingRandomBodyWhilePressed, new bool(true));
	}

	if(btn == btnRemove)
	{
		planetarium->removeFocusedBodies(true);
	}

	if(btn == btnClear)
	{
		planetarium->removeAllBodies();
	}

	if(btn == btnRecolorAll)
	{
		planetarium->recolorAllBodies();
	}

	if(btn == btnFollowSelection)
	{
		planetarium->setReferenceFrameAsFocusedBodies(true);
	}

	if(btn == btnResetReferenceFrame)
	{
		planetarium->resetReferenceFrame(true);
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
		planetarium->setOrbitTraceLength(planetarium->getOrbitTraceLength()*2.0);
		spnTraceLength->refresh();
	}
	if(btn == btnHalveTraceLentgh)
	{
		planetarium->setOrbitTraceLength(planetarium->getOrbitTraceLength()*0.5);
		spnTraceLength->refresh();
	}

	if(btn == btnUndo)
	{
		planetarium->undoLastChange();
	}

	if(btn == btnRewind)
	{
		planetarium->rewindLastChange();
	}

	if(btn == btnNew)
	{
		Universe2D emptyUniverse;
		emptyUniverse.gravity = DEFAULT_GRAVITY;
		replaceUniverse(emptyUniverse);
	}

	//fixme there's got to be a better way to avoid file_chooser behind FileDialog bug
	//xxx workaround for file_chooser behind FileDialog bug
	if(btn == btnLoad or btn == btnSave)
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

	if(chck == chckTraceOrbit or chck == tgbTraceOrbit) //update each other graphics as they share value
	{
		tgbTraceOrbit->draw_blit_upd();
		chckTraceOrbit->draw_blit_upd();
	}

	if(chck == chckBouncingOnCollision)
	{
		planetarium->physics->collisionMode = chckBouncingOnCollision->value()? Physics2D::BOUNCE_ON_COLLISION : Physics2D::MERGE_ON_COLLISION;
	}

	if(chck == chckComputeEnergy)
	{
		if(not planetarium->physics->systemEnergyComputingEnabled)
		{
			msgLogK->draw_mes("--");
			msgLogP->draw_mes("--");
			msgLogE->draw_mes("--");
		}
	}

	if(chck == tglCollapseLeftPanel)
	{
		collapseTabs(*tglCollapseLeftPanel->d);
	}

	if(chck == tglFullScreen)
	{
		toogleFullscreen(*tglFullScreen->d);
	}
}

void onDropDownMenuButton(RButWin* btn, int nr, int fire)
{
	if(btn == ddmTraceStyle->cmdMenu->buttons && fire)
	{
		RButton* rbtn = btn->act_button();
		if(string(rbtn->label.str) == "Linear")
		{
			planetarium->setOrbitTraceStyle(Planetarium::LINEAR);
			ddmTraceStyle->cmdMenu->src->label = "Linear";
			ddmTraceStyle->cmdMenu->src->draw_blit_upd();
		}
		if(string(rbtn->label.str) == "Dotted")
		{
			planetarium->setOrbitTraceStyle(Planetarium::DOTTED);
			ddmTraceStyle->cmdMenu->src->label = "Dotted";
			ddmTraceStyle->cmdMenu->src->draw_blit_upd();
		}
	}

	if(btn == ddmIntegrationMethod->cmdMenu->buttons && fire)
	{
		RButton* rbtn = btn->act_button();
		if(planetarium->physics->solver->factory->solverDisplayName == string(rbtn->label.str))
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

		if(selectedSolverFactory != null)
		{
			planetarium->setSolver(selectedSolverFactory->createSolver(*planetarium->physics));
			spnTimeStep->setValue(&planetarium->physics->solver->timestep); //updates the backing value
			spnGExp->setValue(&(planetarium->physics->universe.gExp)); //updates the backing value
		}

		ddmIntegrationMethod->cmdMenu->src->label = rbtn->label.str;
		ddmIntegrationMethod->cmdMenu->src->draw_blit_upd();
		spnTimeStep->refresh();
		spnGExp->refresh();
	}
}

void onFileChosenOpenUniverse(FileDialog* dialog)
{
	if(not dialog->selectedFilename.empty())
	{
		try
		{
			loadUniverseFromFile(dialog->selectedFilename);
		}
		catch (std::exception& e)
		{
			alert(e.what());
		}
	}
	onButtonPressed(btnRun); // todo make this optional
	SDL_WM_SetCaption("cplanets", "cplanets"); //todo maybe put filename on title
}

void onFileChosenSaveUniverse(FileDialog* dialog)
{
	if(not dialog->selectedFilename.empty())
	{
		// query 'dialog->selectedType' for selected file type
		ApplicationIO::save(planetarium->physics->universe, dialog->selectedFilename);
	}
	onButtonPressed(btnRun);
	SDL_WM_SetCaption("cplanets", "cplanets"); //todo maybe put filename on title
}

void onUserEvent(int cmd,int param,int param2)
{
	if(cmd == PlanetariumPane::USER_EVENT_ID__REDRAW_REQUESTED)
	{
		if(param == planetariumPane->id.id1) //kind of unnecessary, we currently have only one instance of planetarium
		{
			planetariumPane->doRefresh();

			if(*tglFullScreen->d == false) // dont draw this if fullscreen
			{
				if(planetarium->physics->systemEnergyComputingEnabled)
				{
					msgLogK->draw_mes("%.4g", log10(planetarium->physics->totalKineticEnergy));
					msgLogP->draw_mes("%.4g", log10(planetarium->physics->totalPotentialEnergy));
					msgLogE->draw_mes("%.4g", log10(planetarium->physics->totalKineticEnergy+planetarium->physics->totalPotentialEnergy));
				}
				msgBodyCount->draw_mes("%u", planetarium->physics->bodyCount);
			}
		}
	}

	if(cmd == ::USER_EVENT_ID__UPDATE_BODIES_LIST)
	{
		txtBodiesUpdateSize();
		sclpBodies->refresh();
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

void onBodyReFocus()
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

void onBodyDeletion(Body2D* deletedBodyPtr)
{
	vector<Body2DClone>* data = txtBodies->getListData();
	foreach(Body2DClone&, b, vector<Body2DClone>, *data)
		if(b.original == deletedBodyPtr)
		{
			txtBodiesRefreshAll();
			return;
		}
}

void txtBodiesRefreshAll()
{
	txtBodies->setListData(new vector<Body2DClone>(planetarium->getBodies()), true);
	send_uev(::USER_EVENT_ID__UPDATE_BODIES_LIST);
}

void txtBodiesUpdateSize()
{
	unsigned height2 = sclpBodies->tw_area.h;

	if(txtBodies->size() >= 0) //if there are texts, make the height of the content to be at least the needed size for the texts
		height2 = max(txtBodies->getListHeight(), (unsigned) sclpBodies->tw_area.h);

	if(height2 != txtBodies->tw_area.h) //avoids unneeded widening
	{
		txtBodies->widen(0, height2 - txtBodies->tw_area.h);
		sclpBodies->widenContent(0, height2 - sclpBodies->content.tw_area.h);
	}
}

void dialogAboutAdjust()
{
	const int headerSize = dialogAbout->titleBarArea.h + TTF_FontHeight(draw_title_ttf->ttf_font)*2;
	const int totalSize = headerSize + mltAboutText->getTextHeight();

	//expand BgrWin to fit the text
	if(totalSize > sclpAboutLicense->content.tw_area.h)
	{
		sclpAboutLicense->widenContent(0, totalSize - sclpAboutLicense->content.tw_area.h);
		sclpAboutLicense->refresh();
	}
}

void collapseTabs(bool choice)
{
	if(choice == true)
	{
		planetariumPane->move(-tabs->tw_area.w, 0);
		planetariumPane->widen(tabs->tw_area.w, 0);
		tabs->hide();
		planetarium->viewportPosition.x -= tabs->tw_area.w; // maintains viewport position in relation to the screen
	}
	else
	{
		planetariumPane->widen(-tabs->tw_area.w, 0);
		planetariumPane->move(tabs->tw_area.w, 0);
		tabs->show();
		planetarium->viewportPosition.x += tabs->tw_area.w; // maintains viewport position in relation to the screen
		forceFullWindowRefresh();
	}
}

void toogleFullscreen(bool choice)
{
	if(choice == true)
	{
		planetariumPane->move(-tabs->tw_area.w-2*WIDGETS_SPACING, -TOOLBAR_SIZE-WIDGETS_SPACING);
		planetariumPane->widen(tabs->tw_area.w+TOOLBAR_SIZE+3*WIDGETS_SPACING, 2*TOOLBAR_SIZE+2*WIDGETS_SPACING);
		tabs->hide();
		toolbarNorthLayout->hideAll();
		toolbarRight->hideAll();
		toolbarSouthLayout->hideAll();
		planetarium->viewportPosition.x -= tabs->tw_area.w + 2*WIDGETS_SPACING; // maintains viewport position in relation to the screen
		planetarium->viewportPosition.y -= TOOLBAR_SIZE + WIDGETS_SPACING; // maintains viewport position in relation to the screen
	}
	else
	{
		if(planetarium->physics->systemEnergyComputingEnabled)
			onCheckBoxPressed(chckComputeEnergy, false);

		planetariumPane->widen(-tabs->tw_area.w-TOOLBAR_SIZE-3*WIDGETS_SPACING, -2*TOOLBAR_SIZE-2*WIDGETS_SPACING);
		planetariumPane->move(tabs->tw_area.w+2*WIDGETS_SPACING, TOOLBAR_SIZE+WIDGETS_SPACING);
		tabs->show();
		toolbarNorthLayout->showAll();
		toolbarRight->showAll();
		toolbarSouthLayout->showAll();
		planetarium->viewportPosition.x += tabs->tw_area.w + 2*WIDGETS_SPACING; // maintains viewport position in relation to the screen
		planetarium->viewportPosition.y += TOOLBAR_SIZE + WIDGETS_SPACING; // maintains viewport position in relation to the screen
		forceFullWindowRefresh();
	}
}

void loadUniverseFromFile(const string& path)  // throws std::runtime_error
{
	if(FileInputStream(path.c_str()).good())
	{
		Universe2D* u = ApplicationIO::load(path, ApplicationIO::FORMAT_DEFAULT);

		if(u != null)
		{
			replaceUniverse(*u);
			delete u; //after copied, we don't need it
		}

		else throw std::runtime_error("Invalid file: " + path);
	}
	else throw std::runtime_error("File doesn't exist or isn't readable: " + path);
}

void replaceUniverse(const Universe2D& universeCopy)
{
	if(not sdl_running)
	{
		planetarium->setUniverse(universeCopy);
		return;
	}

	onButtonPressed(btnPause);

	planetarium->setUniverse(universeCopy);

	spnGravity->setValue(&(planetarium->physics->universe.gravity)); // updating reference
	spnGravity->refresh();

	spnTimeStep->setValue(&(planetarium->physics->solver->timestep)); // updating reference as solver have changed
	spnTimeStep->refresh();

	spnGExp->setValue(&(planetarium->physics->universe.gExp));
	spnGExp->refresh();

	txtBodiesRefreshAll();
	tabOptions->draw_blit_recur();

	onButtonPressed(btnRun);
}

void addRandomBody(bool orbiting)
{
	const Vector2D minPos = planetarium->getAntiTransposed(Vector2D()), maxPos = planetarium->getAntiTransposed(Vector2D(planetariumPane->tw_area.w, planetariumPane->tw_area.h));
	const double area[4] = {minPos.x, minPos.y, maxPos.x - minPos.x, maxPos.y - minPos.y};

	if(orbiting)
		planetarium->addRandomOrbitingBody(area);
	else
		planetarium->addRandomBody(area);
}

int keepAddingRandomBodyWhilePressed(void* boolCreateOrbitingAsVoidPtr)
{
	SDL_Delay(500); // todo make this delay adjustable
	long lastUpdateTime;
	bool* createOrbiting = static_cast<bool*>(boolCreateOrbitingAsVoidPtr);
	while(aux_isPressed_SDLK_r)
	{
		lastUpdateTime = SDL_GetTicks();
		addRandomBody(*createOrbiting);
		SDL_Delay(5000/planetarium->fps - (SDL_GetTicks() - lastUpdateTime));
	}
	delete createOrbiting;
	return 0;
}
