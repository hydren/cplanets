/*
 * layout.hpp
 *
 *  Created on: 15 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_ABSTRACT_LAYOUT_HPP_
#define WIDGETS_ABSTRACT_LAYOUT_HPP_

#include <vector>

#include "SDL_widgets/SDL_widgets.h"
#include "futil/futil.hpp"

namespace CPlanetsGUI
{
	/** An abstract layout manager. */
	struct Layout
	{
		/** An abstract layout element. It defines basic manipulations. */
		struct Element
		{
			virtual ~Element() {}
			virtual Point getPosition() abstract;
			virtual void setPosition(Point position) abstract;
			virtual Rect getSize() abstract;
			virtual void setSize(Rect size) abstract;
			virtual bool isStretched() abstract;
			virtual bool operator == (const Element& b) abstract;
		};

		/** A WinBase element. It knows how to manipulate WinBase elements. */
		struct WinBaseWrapper extends Element
		{
			WinBase* base;
			WinBaseWrapper(WinBase* winBase);
			virtual ~WinBaseWrapper();
			virtual Point getPosition();
			virtual void setPosition(Point position);
			virtual void setPosition(int x, int y);
			virtual void setX(int x);
			virtual void setY(int y);
			virtual Rect getSize();
			virtual void setSize(Rect size);
			virtual bool isStretched();
			virtual bool operator == (const Element& b);
		};

		Point position;

		/** Creates a layout panel that sits its components from the given coordinates */
		Layout(int x, int y);

		/** Creates a layout panel that sits its components from the given point (copies the point)*/
		Layout(Point& p);

		virtual ~Layout();

		/** Add a component to the layout */
		void addComponent(WinBase& component, int index=-1);
		/** Add a component to the layout */
		void addComponent(WinBase* component, int index=-1);
		/** Add a component to the layout */
		void addComponent(Element& component, int index=-1);
		/** Add a component to the layout */
		void addComponent(Element* component, int index=-1);


		/** Remove the component at the specified index (but does not delete or hides it from the window) */
		void removeComponentAt(unsigned index);
		/** Remove the specified component (but does not free or hides it from the window) */
		void removeComponent(WinBase& component);
		/** Remove the specified component (but does not free or hides it from the window) */
		void removeComponent(WinBase* component);
		/** Remove the specified component (but does not free or hides it from the window) */
		void removeComponent(Element& component);
		/** Remove the specified component (but does not free or hides it from the window) */
		void removeComponent(Element* component);

		/** Organizes the position of the components on this layout */
		virtual void pack() abstract;

		protected:
		std::vector<Element*> components;

		private:
		std::vector<WinBaseWrapper*> innerWrappers;
		void removeIfInnerWrapper(Element* element);
	};
}



#endif /* WIDGETS_ABSTRACT_LAYOUT_HPP_ */
