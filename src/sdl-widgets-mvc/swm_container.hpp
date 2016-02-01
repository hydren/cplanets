/*
 * swm_container.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef SDL_WIDGETS_MVC_SWM_CONTAINER_HPP_
#define SDL_WIDGETS_MVC_SWM_CONTAINER_HPP_

#include <set>

#include "swm_component.hpp"

class SDL_GUIContainer : public SDL_GUIComponent
{
	std::set<SDL_GUIComponent*> childs;

	public:

	SDL_GUIContainer(int x, int y, int width, int height, SDL_GUIContainer* parent=NULL);
	SDL_GUIContainer(float relative_x, float relative_y, int width, int height, SDL_GUIContainer* parent=NULL);
	virtual ~SDL_GUIContainer();

	virtual void add(SDL_GUIComponent* component);
	virtual void remove(SDL_GUIComponent* component);

	virtual void paint();

	virtual void paintComponents();
};



#endif /* SDL_WIDGETS_MVC_SWM_CONTAINER_HPP_ */
