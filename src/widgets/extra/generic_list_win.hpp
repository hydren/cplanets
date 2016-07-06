/*
 * advanced_list_win.hpp
 *
 *  Created on: 5 de jul de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_GENERIC_LIST_WIN_HPP_
#define WIDGETS_GENERIC_LIST_WIN_HPP_

#include "SDL_widgets/SDL_widgets.h"
#include "widgets/list_selection_model.hpp"
#include "list_model.hpp"

namespace WidgetsExtra
{
	struct GenericListWin extends BgrWin
	{
		UIListModel* model;
		ListSelectionModel selection;

		Point padding;
		unsigned spacing;
		RenderText* textRenderer, *textRendererCaseSelected;
		Uint32 bgcolCaseSelected;

		/// Advanced flag used to tweak the behavior of clicking on the list.
		/// If true, the list won't be re-drawed when clicked.
		bool preventRedrawOnClick;

		/// Optional function to be called to adjust this model's selection when changes occur in the list data related to this selection model.
		/// If it is null (default), the selection will be cleared everytime the inner data changes.
		void (*adjustSelection)(ListSelectionModel& selection, const void* oldDataPtr, const void* newDataPtr);

		//can be called only after a TopWin has been constructed
		GenericListWin(WinBase *parent, Style, Rect rect, Id id=0);

		virtual ~GenericListWin();

		virtual void draw();

		virtual void setListData(const std::vector<std::string>& data, bool redrawImmediately=false);
		virtual void setListModel(UIListModel* model, bool redrawImmediately=false, bool deletePrevious=true);
		virtual void updateListData(void* data, bool redrawImmediately=false);
		virtual unsigned getListHeight();
		virtual void clickList(const Point& point);

		protected:
		unsigned lastClickedIndex;
		virtual void onMouseDown(Point point, int buttonNumber);
		virtual void onMouseMove(Point point, int buttonNumber); // by default does nothing
		virtual void onMouseUp(Point point, int buttonNumber); // by default does nothing

		private:
		//compatibility function (with BgrWin pointers)
		static void drawBgrWinAsListWin(BgrWin* bgrWin);
		static void onMouseDown(BgrWin* bgr,int x,int y,int but);
		static void onMouseMove(BgrWin* bgr,int x,int y,int but);
		static void onMouseUp(BgrWin* bgr,int x,int y,int but);
	};
}



#endif /* WIDGETS_GENERIC_LIST_WIN_HPP_ */
