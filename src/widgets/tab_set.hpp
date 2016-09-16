/*
 * tab_set.hpp
 *
 *  Created on: 28 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_TAB_SET_HPP_
#define WIDGETS_TAB_SET_HPP_

#include "SDL_widgets/SDL_widgets.h"
#include "futil/general/language.hpp"
#include "flow_layout.hpp"
#include "tab_control.hpp"

namespace WidgetsExtra
{
	/// A set of tab buttons arranged together. It also contains an inner tab controller.
	struct TabSet
	{
		WinBase* const commonParent;
		TabController controller;
		FlowLayout layout;

		/// Creates a TabSet on the specified position. See FlowLayout for more info on its behavior.
		TabSet(WinBase* parent, int x, int y, unsigned maxWidth=0, unsigned maxHeight=0);

		/// Adds a new tab with the given label and content.
		void addTab(Label lab, BgrWin* content);

		/// Sets the active (visible) tab.
		void setActiveTab(unsigned index);
		void setActiveTab(BgrWin* tabContent);

		static void drawTabStyleBgrWin(BgrWin* bgrWin);
	};
}



#endif /* WIDGETS_TAB_SET_HPP_ */
