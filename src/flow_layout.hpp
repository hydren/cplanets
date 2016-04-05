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

#include "main_window.hpp"

namespace CPlanetsGUI
{
	struct Layout
	{
		Point position;
		std::vector<WinBase*> components;

		/** Creates a layout panel that sits its components from the given coordinates */
		Layout(int x, int y);

		/** Creates a layout panel that sits its components from the given point (copies the point)*/
		Layout(Point& p);

		virtual ~Layout();

		/** Add a component to the layout */
		void addComponent(WinBase& component, int index=-1);
		/** Add a component to the layout */
		void addComponent(WinBase* component, int index=-1);

		/** Remove the component at the specified index (but does not delete or hides it from the window) */
		void removeComponentAt(unsigned index);
		/** Remove the specified component (but does not free or hides it from the window) */
		void removeComponent(WinBase& component);
		/** Remove the specified component (but does not free or hides it from the window) */
		void removeComponent(WinBase* component);

		/** Organizes the position of the components on this layout */
		virtual void pack() abstract;
	};

	struct FlowLayout extends Layout
	{
		FlowLayout(int x, int y);

		FlowLayout(Point& p);

		virtual ~FlowLayout();

		void pack();
	};
}



#endif /* FLOW_LAYOUT_HPP_ */
