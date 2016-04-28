/*
 * tab_set.hpp
 *
 *  Created on: 28 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_TAB_SET_HPP_
#define WIDGETS_TAB_SET_HPP_

#include "SDL_widgets/SDL_widgets.h"
#include "futil/futil.hpp"
#include "flow_layout.hpp"
#include "tab_control.hpp"

namespace SDL_util
{
	/// A set of tab buttons arranged together. It also contains an inner tab controller.
	struct TabSet
	{
		TabController controller;
		FlowLayout layout;

		/// Creates a TabSet on the specified position. See FlowLayout for more info on its behavior.
		TabSet(WinBase* parent, int x, int y, Rect maxSize=Rect(-1,-1,0,0));

		/// Adds a new tab with the given label and content.
		void addTab(Label lab, BgrWin* content);

		/// Sets the active (visible) tab.
		void setActiveTab(unsigned index);
		void setActiveTab(BgrWin* tabContent);

		static void drawTabStyleBgrWin(BgrWin* bgrWin);
	};
}



#endif /* WIDGETS_TAB_SET_HPP_ */
