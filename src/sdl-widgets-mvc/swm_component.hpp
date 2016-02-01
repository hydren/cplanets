/*
 * swm_component.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef SDL_WIDGETS_MVC_SWM_COMPONENT_HPP_
#define SDL_WIDGETS_MVC_SWM_COMPONENT_HPP_

#include <cstdlib>
#include <SDL/SDL.h>

#include "futil/futil.hpp"

class SDL_GUIContainer;

class SDL_GUIComponent
{
	SDL_Rect absoluteBounds;
	float relativePosition[2];
	SDL_GUIContainer* parent;

	public:

	SDL_GUIComponent(int x, int y, int width, int height, SDL_GUIContainer* parent=NULL);
	SDL_GUIComponent(float relative_x, float relative_y, int width, int height, SDL_GUIContainer* parent=NULL);
	virtual ~SDL_GUIComponent();

	virtual void paint() abstract;
};



#endif /* SDL_WIDGETS_MVC_SWM_COMPONENT_HPP_ */
