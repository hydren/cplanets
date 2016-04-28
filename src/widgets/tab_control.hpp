/*
 * tab_control.hpp
 *
 *  Created on: 28 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_TAB_CONTROL_HPP_
#define WIDGETS_TAB_CONTROL_HPP_

#include <vector>

#include "SDL_widgets/SDL_widgets.h"

namespace SDL_util
{
	struct TabController
	{
		WinBase* const commonParent;
		ExtRButCtrl* tabButtonsController;
		std::vector<RExtButton*> tabButtons;
		std::vector<BgrWin*> tabsPanels;

		TabController(WinBase* parent);

		/// The safe way to add tabs with this controller.
		void addTab(Rect tabButtonRect, Label lab, BgrWin* content);
//		void removeTab(unsigned index); //todo do a removeTab() on TabController

		/// Sets the active (visible) tab.
		void setActiveTab(unsigned index);
		void setActiveTab(BgrWin* tabContent);

		private:static void onTabButtonClicked(RExtButton* rbTab, bool is_act);
	};
}



#endif /* WIDGETS_TAB_CONTROL_HPP_ */
