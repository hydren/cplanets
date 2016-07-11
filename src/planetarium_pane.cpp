/*
 * planetarium_pane.cpp
 *
 *  Created on: 11 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium_pane.hpp"

PlanetariumPane::PlanetariumPane(WinBase* parentWidget, Rect rect, Id _id)
: BgrWin(parentWidget, rect, null, PlanetariumPane::drawPlanetarium, PlanetariumPane::onMouseDown, null, PlanetariumPane::onMouseUp, 0, _id),
  SurfaceUpdateDispatcher(), planetarium(new Planetarium(rect))
{
	planetarium->drawDispatcher = this;
	SDL_FreeSurface(planetarium->surf);
	planetarium->surf = null;
}

PlanetariumPane::~PlanetariumPane()
{
	delete planetarium;
}

void PlanetariumPane::doRefresh()
{
	draw_blit_upd();
	planetarium->isRedrawPending = false;
}

void PlanetariumPane::drawPlanetarium(BgrWin* bgr)
{
	PlanetariumPane* self = static_cast<PlanetariumPane*>(bgr);
	self->init_gui();
	self->planetarium->surf = self->win; //make planetarium use this->win to increase performance
	if(self->win == null) return; //if this->win is not ready, skip
	self->planetarium->draw();
}

void PlanetariumPane::onSurfaceUpdate()
{
	send_uev(USER_EVENT_ID__REDRAW_REQUESTED, this->id.id1);
}

void PlanetariumPane::widen(int dx, int dy)
{
	BgrWin::widen(dx, dy);
	planetarium->size.w += dx;
	planetarium->size.h += dy;
}

void PlanetariumPane::onMouseDown(BgrWin* bgr, int x, int y, int but)
{
	static_cast<PlanetariumPane*>(bgr)->planetarium->onMouseButtonPressed(x, y, but);
}

void PlanetariumPane::onMouseUp(BgrWin* bgr, int x, int y, int but)
{
	static_cast<PlanetariumPane*>(bgr)->planetarium->onMouseButtonReleased(x, y, but);
}
