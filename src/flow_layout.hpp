/*
 * flow_layout.hpp
 *
 *  Created on: 28 de mar de 2016
 *      Author: carlosfaruolo
 */

#ifndef FLOW_LAYOUT_HPP_
#define FLOW_LAYOUT_HPP_

#include <vector>
#include <utility>
#include <set>

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

		/** Adds a spacer between before the specified index. If the size is not specified or -1 is used, the spacer will stretch
		 *  to fit the available space on the top window, effectively pushing components after it to the left/right (respecting their sizes).
		 *  This does not affect the component list itself, but the offset between the elements at 'index'-1 and 'index' */
		void addSpacer(int size=-1, int index=-1);

		/** Removes all spacers between 'index'-1 and 'index'.
		 *  This does not affect the component list itself, but the offset between the elements. */
		void removeSpacer(int index);

		private:
		std::set< std::pair<int, int> > spacersSet;
		int getSpacerSize(int index);
		bool needsSpacerStretching() const;
		unsigned computeFreeSpaceOnLayout() const;
	};
}



#endif /* FLOW_LAYOUT_HPP_ */
