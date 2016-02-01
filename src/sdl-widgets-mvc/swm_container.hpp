/*
 * swm_container.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef SDL_WIDGETS_MVC_SWM_CONTAINER_HPP_
#define SDL_WIDGETS_MVC_SWM_CONTAINER_HPP_

#include <list>

#include "swm_component.hpp"

class SDL_GUIContainer : public SDL_GUIComponent
{
	std::list<SDL_GUIComponent*> childs;
	virtual void paint() const;

	public:

	SDL_GUIContainer(int x, int y, int width, int height, SDL_GUIContainer* parent=NULL);
	SDL_GUIContainer(float relative_x, float relative_y, int width, int height, SDL_GUIContainer* parent=NULL);
	virtual ~SDL_GUIContainer();

	/* Adds a component to this container. Every added component will be drawn in order which they were added  */
	virtual void add(SDL_GUIComponent* component);
	/* Removes a component from this container. No other action is taken. */
	virtual void remove(SDL_GUIComponent* component);

	/* Paints all child components of this container. */
	virtual void paintComponents() const;
};



#endif /* SDL_WIDGETS_MVC_SWM_CONTAINER_HPP_ */
