/*
 * planetarium_pane.cpp
 *
 *  Created on: 11 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium_pane.hpp"

PlanetariumPane::PlanetariumPane(WinBase* parentWidget, Rect rect, Id _id)
: BgrWin(parentWidget, rect, null, PlanetariumPane::drawPlanetarium, PlanetariumPane::onMouseDown, null, PlanetariumPane::onMouseUp, 0, _id),
  SurfaceUpdateDispatcher(), impl(new Planetarium(rect))
{
	impl->drawDispatcher = this;
}

PlanetariumPane::~PlanetariumPane()
{
	delete impl;
}

void PlanetariumPane::doRefresh()
{
	draw_blit_upd();
	impl->isRedrawPending = false;
}

void PlanetariumPane::drawPlanetarium(BgrWin* bgr)
{
	PlanetariumPane* self = static_cast<PlanetariumPane*>(bgr);
	self->init_gui();
	self->impl->draw();
	SDL_BlitSurface(self->impl->surf, null, self->win, null);
}

void PlanetariumPane::onSurfaceUpdate()
{
	send_uev(USER_EVENT_ID__REDRAW_REQUESTED, this->id.id1);
}

void PlanetariumPane::widen(int dx, int dy)
{
	BgrWin::widen(dx, dy);
	impl->widen(dx, dy);
}

void PlanetariumPane::onMouseDown(BgrWin* bgr, int x, int y, int but)
{
	static_cast<PlanetariumPane*>(bgr)->impl->onMouseButtonPressed(x, y, but);
}

void PlanetariumPane::onMouseUp(BgrWin* bgr, int x, int y, int but)
{
	static_cast<PlanetariumPane*>(bgr)->impl->onMouseButtonReleased(x, y, but);
}
