/*
 * flow_layout.hpp
 *
 *  Created on: 28 de mar de 2016
 *      Author: carlosfaruolo
 */

#ifndef FLOW_LAYOUT_HPP_
#define FLOW_LAYOUT_HPP_


#include "SDL_widgets/SDL_widgets.h"

#include "abstract_layout.hpp"

namespace CPlanetsGUI
{
	/** A layout that places its elements side by side, with a minimum spacing.
	 *  If it contains a stretching component, the component is stretch until reach this layout maxSize.
	 *  If maxSize is zero, the layout uses all the space after its position.*/
	struct FlowLayout extends Layout
	{
		unsigned maxSize;

		FlowLayout(int x, int y, int max=0);

		FlowLayout(Point p);

		virtual ~FlowLayout();

		void pack();

		private:
		bool needsStretching() const; //check if there is a stretching element
		unsigned computeFreeSpaceOnLayout() const; //computes the free space to stretch-out
		unsigned getStretchingElementsCount() const; //counts the number of stretching elements
	};
}

#endif /* FLOW_LAYOUT_HPP_ */
