/*
 * planetarium_pane.hpp
 *
 *  Created on: 11 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef PLANETARIUM_PANE_HPP_
#define PLANETARIUM_PANE_HPP_

#include "planetarium.hpp"

#include "SDL_widgets/SDL_widgets.h"
#include "futil/general/language.hpp"

struct PlanetariumPane extends BgrWin, Planetarium::SurfaceUpdateDispatcher
{
	/// SDL_widgets user event ID hinting that it should be redrawed. Catch such events with a 'handle_uev' function and call 'doRefresh()'.
	static const int USER_EVENT_ID__REDRAW_REQUESTED = 192;

	Planetarium* planetarium;
	RenderText* renderTextPaused;
	Point pointTextPaused;

	bool pauseEffectsEnabled;

	PlanetariumPane(WinBase* parentWidget, Rect rect, Id _id=0);
	virtual ~PlanetariumPane();

	virtual void widen(int dx,int dy);
	virtual void move(int delta_x, int delta_y);

	/** It's not recommended to call these directly. */
	virtual void doRefresh();
	virtual void onSurfaceUpdate();

	static void drawPlanetarium(BgrWin* bgr);
	static void onMouseDown(BgrWin* bgr, int x, int y, int but);
	static void onMouseUp(BgrWin* bgr, int x, int y, int but);
};



#endif /* PLANETARIUM_PANE_HPP_ */
