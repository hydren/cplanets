/*
 * swm_component.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef SDL_WIDGETS_MVC_SWM_COMPONENT_HPP_
#define SDL_WIDGETS_MVC_SWM_COMPONENT_HPP_

#include <cstdlib>
#include <sdl-widgets/sdl-widgets.h>

#include "futil/futil.hpp"

class SDL_GUIContainer;

/* The base class for any component. It's abstract, meant to be extended. */
class SDL_GUIComponent
{
	/* The absolute position of this component in relation to its parent's position. */
	Point absolutePosition;
	/* The relative position of this component in relation to its parent's bounds. */
	float relativePosition[2];
	/* This component's dimension. */
	int width, height;

	/** If true, allows this component to be pained. */
	bool visible;

	/* Colors used to do basic painting. */
	SDL_Color backgroundColor, foregroundColor;

	/** This component's parent container.
	 * If the parent is assigned as the component itself or null, it indicated that this component doesn't have a parent and it should be positioned in relation to the application window. */
	SDL_GUIContainer* parent;

	/* A method that paints this component on the window. */
	virtual void paint() const;

	public:

	/* Constructs a component with the given bounds and parent containter. */
	SDL_GUIComponent(int x, int y, int width, int height, SDL_GUIContainer* parent=NULL);
	/* Constructs a component with the given relative position (ratio of its parent's bounds) and parent containter. */
	SDL_GUIComponent(float relative_x, float relative_y, int width, int height, SDL_GUIContainer* parent=NULL);
	virtual ~SDL_GUIComponent();

	int getWidth() const;
	int getHeight() const;
	int getX() const;
	int getY() const;
	SDL_Rect getBounds() const;
	int getCenterX() const;
	int getCenterY() const;

	float getRelativeX() const;
	float getRelativeY() const;
	/* Returns an 2-position array with respectively this components relative X and relative Y position. */
	float* getRelativePosition() const;

	void paintComponent() const;
};



#endif /* SDL_WIDGETS_MVC_SWM_COMPONENT_HPP_ */
