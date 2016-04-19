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
		//this layout bound. x and y attributes are ignored. If w==0, then it goes until the right border. If h==0, then it goes until the bottom.
		Rect maxSize;
		unsigned spacing_h, spacing_v; //the spacing between components. default is DEFAULT_SPACING.
		enum Alignment {BEFORE, MIDDLE, AFTER} alignment; //the alignment of the components on the perpendicular axis.

		FlowLayout(int x, int y, Rect max=Rect(-1,-1,0,0));

		FlowLayout(Point p);

		virtual ~FlowLayout();

		void pack();

		static const unsigned DEFAULT_SPACING = 4;

		private:
		bool needsStretching() const; //check if there is a stretching element
		unsigned computeFreeSpaceOnLayout() const; //computes the free space to stretch-out
		unsigned getStretchingElementsCount() const; //counts the number of stretching elements
		int computeAlignment(Element* e, int pos) const;
	};
}

#endif /* FLOW_LAYOUT_HPP_ */
