/*
 * planetarium_pane.cpp
 *
 *  Created on: 11 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium_pane.hpp"
#include "SDL_util.hpp"

PlanetariumPane::PlanetariumPane(WinBase* parentWidget, Rect rect, Id _id)
: BgrWin(parentWidget, rect, null, PlanetariumPane::drawPlanetarium, PlanetariumPane::onMouseDown, null, PlanetariumPane::onMouseUp, 0, _id),
  SurfaceUpdateDispatcher(), planetarium(new Planetarium(rect)), renderTextPaused(new RenderText(draw_title_ttf->ttf_font, SDL_util::Color::ORANGE)),
  pauseEffectsEnabled(true)
{
	planetarium->drawDispatcher = this;
	SDL_FreeSurface(planetarium->surf);
	planetarium->surf = null;
}

PlanetariumPane::~PlanetariumPane()
{
	delete planetarium;
}

void PlanetariumPane::widen(int dx, int dy)
{
	BgrWin::widen(dx, dy);
	planetarium->size.w += dx;
	planetarium->size.h += dy;
}

void PlanetariumPane::move(int delta_x, int delta_y)
{
	BgrWin::move(delta_x, delta_y);
	planetarium->pos.x += delta_x;
	planetarium->pos.y += delta_y;
}

void PlanetariumPane::doRefresh()
{
	draw_blit_upd();
	planetarium->isRedrawPending = false;
}

void PlanetariumPane::onSurfaceUpdate()
{
	send_uev(USER_EVENT_ID__REDRAW_REQUESTED, this->id.id1);
}

void PlanetariumPane::drawPlanetarium(BgrWin* bgr)
{
	PlanetariumPane* self = static_cast<PlanetariumPane*>(bgr);
	self->init_gui();
	self->planetarium->surf = self->win; //make planetarium use this->win to increase performance
	if(self->win == null) return; //if this->win is not ready, skip
	self->planetarium->draw();

	if(self->planetarium->running == false and self->pauseEffectsEnabled) // draw this when paused
	{
		rectangleRGBA(self->win, 0, 0, self->tw_area.w-1, self->tw_area.h-1, self->renderTextPaused->text_col.r, self->renderTextPaused->text_col.g, self->renderTextPaused->text_col.b, 255);
		rectangleRGBA(self->win, 1, 1, self->tw_area.w-2, self->tw_area.h-2, self->renderTextPaused->text_col.r, self->renderTextPaused->text_col.g, self->renderTextPaused->text_col.b, 255);
		self->renderTextPaused->draw_string(self->win, "PAUSED", Point(8, 8));
	}
}

void PlanetariumPane::onMouseDown(BgrWin* bgr, int x, int y, int but)
{
	static_cast<PlanetariumPane*>(bgr)->planetarium->onMouseButtonPressed(x, y, but);
}

void PlanetariumPane::onMouseUp(BgrWin* bgr, int x, int y, int but)
{
	static_cast<PlanetariumPane*>(bgr)->planetarium->onMouseButtonReleased(x, y, but);
}
