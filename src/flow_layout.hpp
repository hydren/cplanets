/*
 * flow_layout.hpp
 *
 *  Created on: 28 de mar de 2016
 *      Author: carlosfaruolo
 */

#ifndef FLOW_LAYOUT_HPP_
#define FLOW_LAYOUT_HPP_

#include <vector>

#include "SDL_widgets/SDL_widgets.h"

namespace CPlanetsGUI
{
	struct FlowLayoutPanel
	{
		std::vector<WinBase*> components;

		void addComponent(WinBase& component, int index=-1);
		void addComponent(WinBase* component, int index=-1);

		void removeComponentAt(unsigned index);
		void removeComponent(WinBase& component);
		void removeComponent(WinBase* component);

		void pack();
	};
}



#endif /* FLOW_LAYOUT_HPP_ */
