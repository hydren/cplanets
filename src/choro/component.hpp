/*
 * component.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef CHORO_COMPONENT_HPP_
#define CHORO_COMPONENT_HPP_

#include "sdl-widgets/sdl-widgets.h"

class CComponent
{
	WinBase* base;

	public:

	bool contains(const int x, const int y) const;
	bool contains(const Point& point) const;
//	Graphics getGraphics const;
	SDL_Color getBackground() const;
	SDL_Color getForeground() const;
	TTF_Font getFont() const;
	Rect getBounds() const;
	int getHeight() const;
	int getWidth() const;
	//TODO more functions
	//...
};



#endif /* CHORO_COMPONENT_HPP_ */
