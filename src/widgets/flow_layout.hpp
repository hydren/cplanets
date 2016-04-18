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
	struct FlowLayout extends Layout
	{
		unsigned maxSize;

		FlowLayout(int x, int y, int max=0);

		FlowLayout(Point p);

		virtual ~FlowLayout();

		void pack();

		private:
		bool needsStretching() const;
		unsigned computeFreeSpaceOnLayout() const;
		unsigned getStretchingElementsCount() const;
	};
}

#endif /* FLOW_LAYOUT_HPP_ */
